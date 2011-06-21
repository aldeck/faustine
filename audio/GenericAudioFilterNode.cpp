// GenericAudioFilterNode.cpp
// e.moon 16jun99

#include "GenericAudioFilterNode.h"

#include "SoundUtils.h"

#include <Buffer.h>
#include <BufferGroup.h>
#include <ByteOrder.h>
#include <Debug.h>
#include <ParameterWeb.h>
#include <TimeSource.h>

#include <Message.h>
#include <Application.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>


// -------------------------------------------------------- //
// constants 
// -------------------------------------------------------- //

// input-ID symbols
enum input_id_t {
	ID_AUDIO_INPUT
};
	
// output-ID symbols
enum output_id_t {
	ID_AUDIO_MIX_OUTPUT
	//ID_AUDIO_WET_OUTPUT ...
};
	
// parameter ID
enum param_id_t {
	P_MIX_RATIO_LABEL	= 100,
	P_MIX_RATIO,
		
	P_SWEEP_RATE_LABEL	= 200,
	P_SWEEP_RATE,
		
	P_DELAY_LABEL		= 300,
	P_DELAY,
		
	P_DEPTH_LABEL		= 400,
	P_DEPTH,
		
	P_FEEDBACK_LABEL	= 500,
	P_FEEDBACK
};

const char* const GenericAudioFilterNode::kNodeName = "Faustine";


GenericAudioFilterNode::GenericAudioFilterNode(FaustDSP* faustDSP)
	:
	BMediaNode(kNodeName),
	BBufferConsumer(B_MEDIA_RAW_AUDIO),
	BBufferProducer(B_MEDIA_RAW_AUDIO),
	BControllable(),
	BMediaEventLooper(),	
	fOutputEnabled(true),
	fDownstreamLatency(0),
	fProcessingLatency(0),
	fFaustDSP(faustDSP)
{
}


GenericAudioFilterNode::~GenericAudioFilterNode()
{	
	Quit();	
}

	
// #pragma mark - BMediaNode


status_t
GenericAudioFilterNode::HandleMessage(int32 code, const void* pData, size_t size)
{
	// pass off to each base class
	if(
		BBufferConsumer::HandleMessage(code, pData, size) &&
		BBufferProducer::HandleMessage(code, pData, size) &&
		BControllable::HandleMessage(code, pData, size) &&
		BMediaNode::HandleMessage(code, pData, size))
		BMediaNode::HandleBadMessage(code, pData, size);
	
	// +++++ return error on bad message?
	return B_OK;
}


BMediaAddOn*
GenericAudioFilterNode::AddOn(int32* outInternalID) const
{
	return NULL;
		// must return NULL to tell this Node is not part of an AddOn
}


void
GenericAudioFilterNode::SetRunMode(run_mode mode)
{
	// disallow offline mode for now
	// +++++
	if(mode == B_OFFLINE)
		ReportError(B_NODE_FAILED_SET_RUN_MODE);
		
	// +++++ any other work to do?
	
	// hand off
	BMediaEventLooper::SetRunMode(mode);
}


// "The Media Server calls this hook function after the node has
//  been registered.  This is derived from BMediaNode; BMediaEventLooper
//  implements it to call Run() automatically when the node is registered;
//  if you implement NodeRegistered() you should call through to
//  BMediaEventLooper::NodeRegistered() after you've done your custom 
//  operations."

float GenericAudioFilterNode::fMarcel = 0.0f;

void
GenericAudioFilterNode::NodeRegistered()
{
	PRINT(("GenericAudioFilterNode::NodeRegistered()\n"));
	
	// Start the BMediaEventLooper thread
	SetPriority(B_REAL_TIME_PRIORITY);
	Run();

	// figure preferred ('template') format
	fPreferredFormat.type = B_MEDIA_RAW_AUDIO;
	_GetPreferredFormat(fPreferredFormat);
	
	// initialize current format
	fFormat.type = B_MEDIA_RAW_AUDIO;
	fFormat.u.raw_audio = media_raw_audio_format::wildcard;
	
	// init input
	fInput.destination.port = ControlPort();
	fInput.destination.id = ID_AUDIO_INPUT;
	fInput.node = Node();
	fInput.source = media_source::null;
	fInput.format = fFormat;
	strncpy(fInput.name, "Audio Input", B_MEDIA_NAME_LENGTH);
	
	// init output
	fOutput.source.port = ControlPort();
	fOutput.source.id = ID_AUDIO_MIX_OUTPUT;
	fOutput.node = Node();
	fOutput.destination = media_destination::null;
	fOutput.format = fFormat;
	strncpy(fOutput.name, "Mix Output", B_MEDIA_NAME_LENGTH);

	// init parameters
	_InitParameterValues();
	_InitParameterWeb();
}
	

// #pragma mark - BMediaEventLooper


void
GenericAudioFilterNode::HandleEvent(const media_timed_event* event, bigtime_t howLate,
	bool realTimeEvent)
{
	ASSERT(event);
	
	switch(event->type) {
		case BTimedEventQueue::B_PARAMETER:
			_HandleParameterEvent(event);
			break;
			
		case BTimedEventQueue::B_START:
			_HandleStartEvent(event);
			break;
			
		case BTimedEventQueue::B_STOP:
			_HandleStopEvent(event);
			break;
			
		default:
			_IgnoreEvent(event);
			break;
	}
}

	
// "Augment OfflineTime() to compute the node's current time; it's called
//  by the Media Kit when it's in offline mode. Update any appropriate
//  internal information as well, then call through to the BMediaEventLooper
//  implementation."
bigtime_t
GenericAudioFilterNode::OfflineTime()
{
	// +++++
	return 0LL;
}


// #pragma mark - BBufferConsumer


status_t
GenericAudioFilterNode::AcceptFormat(const media_destination& destination,
	media_format* pioFormat)
{

	PRINT(("GenericAudioFilterNode::AcceptFormat()\n"));

	// sanity checks
	if(destination != fInput.destination) {
		PRINT(("\tbad destination\n"));
		return B_MEDIA_BAD_DESTINATION;
	}
	if(pioFormat->type != B_MEDIA_RAW_AUDIO) {
		PRINT(("\tnot B_MEDIA_RAW_AUDIO\n"));
		return B_MEDIA_BAD_FORMAT;
	}

	_ValidateProposedFormat(
		(fFormat.u.raw_audio.format != media_raw_audio_format::wildcard.format) ?
			fFormat : fPreferredFormat,
		*pioFormat);
	return B_OK;
}

	
// "If you're writing a node, and receive a buffer with the B_SMALL_BUFFER
//  flag set, you must recycle the buffer before returning."	
void
GenericAudioFilterNode::BufferReceived(BBuffer* buffer)
{
	ASSERT(buffer);

	// check buffer destination
	if(buffer->Header()->destination !=
		fInput.destination.id) {
		PRINT(("GenericAudioFilterNode::BufferReceived():\n"
			"\tBad destination.\n"));
		buffer->Recycle();
		return;
	}
	
	if(buffer->Header()->time_source != TimeSource()->ID()) {
		PRINT(("* timesource mismatch\n"));
	}

	// check output
	if(fOutput.destination == media_destination::null ||
		!fOutputEnabled) {
		buffer->Recycle();
		return;
	}
		
	// process and retransmit buffer
	_FilterBuffer(buffer);		

	status_t err = SendBuffer(buffer, fOutput.source, fOutput.destination);
	if(err < B_OK) {
		PRINT(("GenericAudioFilterNode::BufferReceived():\n"
			"\tSendBuffer() failed: %s\n", strerror(err)));
		buffer->Recycle();
	}
	// sent!
}
	

// * make sure to fill in poInput->format with the contents of
//   pFormat; as of R4.5 the Media Kit passes poInput->format to
//   the producer in BBufferProducer::Connect().
status_t
GenericAudioFilterNode::Connected(const media_source& source,
	const media_destination& destination, const media_format& format,
	media_input* poInput)
{
	
	PRINT(("GenericAudioFilterNode::Connected()\n"
		"\tto source %ld\n", source.id));
	
	// sanity check
	if(destination != fInput.destination) {
		PRINT(("\tbad destination\n"));
		return B_MEDIA_BAD_DESTINATION;
	}
	if(fInput.source != media_source::null) {
		PRINT(("\talready connected\n"));
		return B_MEDIA_ALREADY_CONNECTED;
	}
	
	// initialize input
	fInput.source = source;
	fInput.format = format;
	*poInput = fInput;
	
	// store format (this now constrains the output format)
	fFormat = format;
	
	return B_OK;
}


void
GenericAudioFilterNode::Disconnected(const media_source& source,
	const media_destination& destination)
{
	PRINT(("GenericAudioFilterNode::Disconnected()\n"));

	// sanity checks
	if(fInput.source != source) {
		PRINT(("\tsource mismatch: expected ID %ld, got %ld\n",
			fInput.source.id, source.id));
		return;
	}
	if(destination != fInput.destination) {
		PRINT(("\tdestination mismatch: expected ID %ld, got %ld\n",
			fInput.destination.id, destination.id));
		return;
	}

	// mark disconnected
	fInput.source = media_source::null;
	
	// no output? clear format:
	if(fOutput.destination == media_destination::null) {
		fFormat.u.raw_audio = media_raw_audio_format::wildcard;
	}
	
	fInput.format = fFormat;
}

		
void
GenericAudioFilterNode::DisposeInputCookie(int32 cookie)
{
}

	
// "You should implement this function so your node will know that the data
//  format is going to change. Note that this may be called in response to
//  your AcceptFormat() call, if your AcceptFormat() call alters any wildcard
//  fields in the specified format. 
//
//  Because FormatChanged() is called by the producer, you don't need to (and
//  shouldn't) ask it if the new format is acceptable. 
//
//  If the format change isn't possible, return an appropriate error from
//  FormatChanged(); this error will be passed back to the producer that
//  initiated the new format negotiation in the first place."
status_t
GenericAudioFilterNode::FormatChanged(const media_source& source,
	const media_destination& destination, int32 changeTag,
	const media_format& newFormat)
{
	// flat-out deny format changes
	return B_MEDIA_BAD_FORMAT;
}
	
		
status_t
GenericAudioFilterNode::GetLatencyFor(const media_destination& destination,
	bigtime_t* poLatency, media_node_id* poTimeSource)
{
	PRINT(("GenericAudioFilterNode::GetLatencyFor()\n"));
	
	// sanity check
	if(destination != fInput.destination) {
		PRINT(("\tbad destination\n"));
		return B_MEDIA_BAD_DESTINATION;
	}
	
	*poLatency = fDownstreamLatency + fProcessingLatency;
	PRINT(("\treturning %Ld\n", *poLatency));
	*poTimeSource = TimeSource()->ID();
	return B_OK;
}

		
status_t
GenericAudioFilterNode::GetNextInput(int32* pioCookie, media_input* poInput)
{
	if(*pioCookie)
		return B_BAD_INDEX;
	
	++*pioCookie;
	*poInput = fInput;
	return B_OK;
}


void
GenericAudioFilterNode::ProducerDataStatus(const media_destination& destination, int32 status,
	bigtime_t tpWhen)
{
	PRINT(("GenericAudioFilterNode::ProducerDataStatus()\n"));
	
	// sanity check
	if(destination != fInput.destination) {
		PRINT(("\tbad destination\n"));
	}
	
	if(fOutput.destination != media_destination::null) {
		// pass status downstream
		status_t err = SendDataStatus(
			status, 
			fOutput.destination,
			tpWhen);
		if(err < B_OK) {
			PRINT(("\tSendDataStatus(): %s\n", strerror(err)));
		}
	}
}

	
// "This function is provided to aid in supporting media formats in which the
//  outer encapsulation layer doesn't supply timing information. Producers will
//  tag the buffers they generate with seek tags; these tags can be used to
//  locate key frames in the media data."
status_t
GenericAudioFilterNode::SeekTagRequested(const media_destination& destination,
	bigtime_t targetTime, uint32 flags,	media_seek_tag* poSeekTag,
	bigtime_t* poTaggedTime, uint32* poFlags)
{
	PRINT(("GenericAudioFilterNode::SeekTagRequested()\n"
		"\tNot implemented.\n"));
	return B_ERROR;
}
	
// -------------------------------------------------------- //
// *** BBufferProducer
// -------------------------------------------------------- //
// #pragma mark - BBufferProducer

// "When a consumer calls BBufferConsumer::RequestAdditionalBuffer(), this
//  function is called as a result. Its job is to call SendBuffer() to
//  immediately send the next buffer to the consumer. The previousBufferID,
//  previousTime, and previousTag arguments identify the last buffer the
//  consumer received. Your node should respond by sending the next buffer
//  after the one described. 
//
//  The previousTag may be NULL.
//  Return B_OK if all is well; otherwise return an appropriate error code."
void
GenericAudioFilterNode::AdditionalBufferRequested(const media_source& source,
	media_buffer_id previousBufferID, bigtime_t previousTime,
	const media_seek_tag* pPreviousTag)
{
	PRINT(("GenericAudioFilterNode::AdditionalBufferRequested\n"
		"\tOffline mode not implemented."));
}

		
void
GenericAudioFilterNode::Connect(status_t status, const media_source& source,
	const media_destination& destination, const media_format& format, char* pioName)
{
	PRINT(("GenericAudioFilterNode::Connect()\n"));
	status_t err;
	
	// connection failed?	
	if(status < B_OK) {
		PRINT(("\tStatus: %s\n", strerror(status)));
		// 'unreserve' the output
		fOutput.destination = media_destination::null;
		return;
	}
	
	// connection established:
	strncpy(pioName, fOutput.name, B_MEDIA_NAME_LENGTH);
	fOutput.destination = destination;
	fFormat = format;
	
	// figure downstream latency
	media_node_id timeSource;
	err = FindLatencyFor(fOutput.destination, &fDownstreamLatency, &timeSource);
	if(err < B_OK) {
		PRINT(("\t!!! FindLatencyFor(): %s\n", strerror(err)));
	}
	PRINT(("\tdownstream latency = %Ld\n", fDownstreamLatency));
	
	// prepare the filter
	_InitFilter();

	// figure processing time
	fProcessingLatency = _CalcProcessingLatency();
	PRINT(("\tprocessing latency = %Ld\n", fProcessingLatency));
	
	// store summed latency
	SetEventLatency(fDownstreamLatency + fProcessingLatency);
	
	if(fInput.source != media_source::null) {
		// pass new latency upstream
		err = SendLatencyChange(
			fInput.source,
			fInput.destination,
			EventLatency() + SchedulingLatency());
		if(err < B_OK)
			PRINT(("\t!!! SendLatencyChange(): %s\n", strerror(err)));
	}

	// cache buffer duration
	SetBufferDuration(
		buffer_duration(
			fFormat.u.raw_audio));
}

		
void
GenericAudioFilterNode::Disconnect(const media_source& source,
	const media_destination& destination)
{
	PRINT(("GenericAudioFilterNode::Disconnect()\n"));
	
	// sanity checks	
	if(source != fOutput.source) {
		PRINT(("\tbad source\n"));
		return;
	}
	if(destination != fOutput.destination) {
		PRINT(("\tbad destination\n"));
		return;
	}
	
	// clean up
	fOutput.destination = media_destination::null;
	
	// no input? clear format:
	if(fInput.source == media_source::null) {
		fFormat.u.raw_audio = media_raw_audio_format::wildcard;
	}
	
	fOutput.format = fFormat;
	
	// +++++ other cleanup goes here
}

		
status_t
GenericAudioFilterNode::DisposeOutputCookie(int32 cookie)
{
	return B_OK;
}

	
void
GenericAudioFilterNode::EnableOutput(const media_source& source, bool enabled,
	int32* _deprecated_)
{
	PRINT(("GenericAudioFilterNode::EnableOutput()\n"));
	if(source != fOutput.source) {
		PRINT(("\tbad source\n"));
		return;
	}
	
	fOutputEnabled = enabled;
}


status_t
GenericAudioFilterNode::FormatChangeRequested(const media_source& source,
	const media_destination& destination, media_format* pioFormat,
	int32* _deprecated_)
{
	// deny
	PRINT(("GenericAudioFilterNode::FormatChangeRequested()\n"
		"\tNot supported.\n"));
		
	return B_MEDIA_BAD_FORMAT;
}

		
status_t
GenericAudioFilterNode::FormatProposal(const media_source& source,	media_format* pioFormat)
{
	PRINT(("GenericAudioFilterNode::FormatProposal()\n"));
	
	if(source != fOutput.source) {
		PRINT(("\tbad source\n"));
		return B_MEDIA_BAD_SOURCE;
	}
	
	if(pioFormat->type != B_MEDIA_RAW_AUDIO) {
		PRINT(("\tbad type\n"));
		return B_MEDIA_BAD_FORMAT;
	}
	
	_ValidateProposedFormat(
		(fFormat.u.raw_audio.format != media_raw_audio_format::wildcard.format) ?
			fFormat :
			fPreferredFormat,
		*pioFormat);
	return B_OK;
}

	
status_t
GenericAudioFilterNode::FormatSuggestionRequested(media_type type,	int32 quality,
	media_format* poFormat)
{
	PRINT(("GenericAudioFilterNode::FormatSuggestionRequested()\n"));
	if(type != B_MEDIA_RAW_AUDIO) {
		PRINT(("\tbad type\n"));
		return B_MEDIA_BAD_FORMAT;
	}
	
	if(fFormat.u.raw_audio.format != media_raw_audio_format::wildcard.format)
		*poFormat = fFormat;
	else
		*poFormat = fPreferredFormat;
	return B_OK;
}

	
status_t
GenericAudioFilterNode::GetLatency(bigtime_t* poLatency)
{
	PRINT(("GenericAudioFilterNode::GetLatency()\n"));
	*poLatency = EventLatency() + SchedulingLatency();
	PRINT(("\treturning %Ld\n", *poLatency));
	
	return B_OK;
}

		
status_t
GenericAudioFilterNode::GetNextOutput(int32* pioCookie, media_output* poOutput)
{
	if(*pioCookie)
		return B_BAD_INDEX;
	
	++*pioCookie;
	*poOutput = fOutput;
	
	return B_OK;
}


// "This hook function is called when a BBufferConsumer that's receiving data
//  from you determines that its latency has changed. It will call its
//  BBufferConsumer::SendLatencyChange() function, and in response, the Media
//  Server will call your LatencyChanged() function.  The source argument
//  indicates your output that's involved in the connection, and destination
//  specifies the input on the consumer to which the connection is linked.
//  newLatency is the consumer's new latency. The flags are currently unused."
void
GenericAudioFilterNode::LatencyChanged(const media_source& source,
	const media_destination& destination, bigtime_t newLatency, uint32 flags)
{
	PRINT(("GenericAudioFilterNode::LatencyChanged()\n"));
	
	if(source != fOutput.source) {
		PRINT(("\tBad source.\n"));
		return;
	}
	if(destination != fOutput.destination) {
		PRINT(("\tBad destination.\n"));
		return;
	}
	
	fDownstreamLatency = newLatency;
	SetEventLatency(fDownstreamLatency + fProcessingLatency);
	
	if(fInput.source != media_source::null) {
		// pass new latency upstream
		status_t err = SendLatencyChange(
			fInput.source,
			fInput.destination,
			EventLatency() + SchedulingLatency());
		if(err < B_OK)
			PRINT(("\t!!! SendLatencyChange(): %s\n", strerror(err)));
	}
}


void
GenericAudioFilterNode::LateNoticeReceived(const media_source& source,	bigtime_t howLate,
	bigtime_t tpWhen)
{
	PRINT(("GenericAudioFilterNode::LateNoticeReceived()\n"
		"\thowLate == %Ld\n"
		"\twhen    == %Ld\n", howLate, tpWhen));
		
	if(source != fOutput.source) {
		PRINT(("\tBad source.\n"));
		return;
	}

	if(fInput.source == media_source::null) {
		PRINT(("\t!!! No input to blame.\n"));
		return;
	}
	
	// +++++ check run mode?
	
	// pass the buck, since this node doesn't schedule buffer
	// production
	NotifyLateProducer(
		fInput.source,
		howLate,
		tpWhen);
}


// PrepareToConnect() is the second stage of format negotiations that happens
// inside BMediaRoster::Connect().  At this point, the consumer's AcceptFormat()
// method has been called, and that node has potentially changed the proposed
// format.  It may also have left wildcards in the format.  PrepareToConnect()
// *must* fully specialize the format before returning!
status_t
GenericAudioFilterNode::PrepareToConnect(const media_source& source,
	const media_destination& destination, media_format* pioFormat,
	media_source* poSource,	char* poName)
{
	char formatStr[256];
	string_for_format(*pioFormat, formatStr, 255);
	PRINT(("GenericAudioFilterNode::PrepareToConnect()\n"
		"\tproposed format: %s\n", formatStr));
	
	if(source != fOutput.source) {
		PRINT(("\tBad source.\n"));
		return B_MEDIA_BAD_SOURCE;
	}
	if(fOutput.destination != media_destination::null) {
		PRINT(("\tAlready connected.\n"));
		return B_MEDIA_ALREADY_CONNECTED;
	}
	
	if(pioFormat->type != B_MEDIA_RAW_AUDIO) {
		PRINT(("\tBad format type.\n"));
		return B_MEDIA_BAD_FORMAT;
	}

	// do a final validity check:
	status_t err = _ValidateProposedFormat(
		(fFormat.u.raw_audio.format != media_raw_audio_format::wildcard.format) ?
			fFormat :
			fPreferredFormat,
		*pioFormat);

	if(err < B_OK) {
		// no go
		return err;
	}
	
	// fill in wildcards
	_SpecializeOutputFormat(*pioFormat);
	
	// reserve the output
	fOutput.destination = destination;
	fOutput.format = *pioFormat;
	
	// pass back source & output name
	*poSource = fOutput.source;
	strncpy(poName, fOutput.name, B_MEDIA_NAME_LENGTH);
	
	return B_OK;
}

		
status_t
GenericAudioFilterNode::SetBufferGroup(const media_source& source,	BBufferGroup* pGroup)
{
	PRINT(("GenericAudioFilterNode::SetBufferGroup()\n"));
	if(source != fOutput.source) {
		PRINT(("\tBad source.\n"));
		return B_MEDIA_BAD_SOURCE;
	}
	
	if(fInput.source == media_source::null) {
		PRINT(("\tNo producer to send buffers to.\n"));
		return B_ERROR;
	}
	
	// +++++ is this right?  buffer-group selection gets
	//       all asynchronous and weird...
	int32 changeTag;
	return SetOutputBuffersFor(
		fInput.source,
		fInput.destination,
		pGroup,
		0, &changeTag);
}

	
status_t
GenericAudioFilterNode::SetPlayRate(int32 numerator, int32 denominator)
{
	// not supported
	return B_ERROR;
}

	
status_t
GenericAudioFilterNode::VideoClippingChanged(const media_source& source, int16 numShorts,
	int16* pClipData, const media_video_display_info& display,
	int32* poFromChangeTag)
{
	// not sane
	return B_ERROR;
}


// #pragma mark - BControllable


status_t
GenericAudioFilterNode::GetParameterValue(int32 id, bigtime_t* poLastChangeTime, void* poValue,
	size_t* pioSize)
{
	//PRINT(("GenericAudioFilterNode::GetParameterValue()\n"));

	// all parameters are floats
	if(*pioSize < sizeof(float)) {
		return B_NO_MEMORY;
	}

	*pioSize = sizeof(float);
	switch(id) {
		/*case P_MIX_RATIO:
			*(float*)poValue = fFlangerMixRatio;
			*poLastChangeTime = m_tpMixRatioChanged;
			break;
		
		case P_SWEEP_RATE:
			*(float*)poValue = m_fSweepRate;
			*poLastChangeTime = m_tpSweepRateChanged;
			break;

		case P_DELAY:
			*(float*)poValue = m_fDelay;
			*poLastChangeTime = m_tpDelayChanged;
			break;
	
		case P_DEPTH:
			*(float*)poValue = m_fDepth;
			*poLastChangeTime = m_tpDepthChanged;
			break;
	
		case P_FEEDBACK:
			*(float*)poValue = m_fFeedback;
			*poLastChangeTime = m_tpFeedbackChanged;
			break;*/
			
		default:
			return B_ERROR;
	}

	return B_OK;
}

		
void
GenericAudioFilterNode::SetParameterValue(int32 id, bigtime_t changeTime, const void* pValue,
	size_t size)
{
	switch(id) {
		case P_MIX_RATIO:
		case P_SWEEP_RATE:
		case P_DELAY:
		case P_DEPTH:
		case P_FEEDBACK: {
			if(size < sizeof(float))
				break;

			media_timed_event ev(
				changeTime,
				BTimedEventQueue::B_PARAMETER,
				0,
				BTimedEventQueue::B_NO_CLEANUP,
				size,
				id,
				(char*)pValue, size);
			EventQueue()->AddEvent(ev);
			break;
		}
	}			
}


void
GenericAudioFilterNode::_HandleParameterEvent(const media_timed_event* pEvent)
{
	float value = *(float*)pEvent->user_data;
	int32 id = pEvent->bigdata;
	size_t size = pEvent->data;
	bigtime_t now = TimeSource()->Now();
	
	switch(id) {
		/*case P_MIX_RATIO:
			if(value == fFlangerMixRatio)
				break;
				
			// set
			fFlangerMixRatio = value;
			m_tpMixRatioChanged = now;
			// broadcast
			BroadcastNewParameterValue(
				now,
				id,
				&fFlangerMixRatio,
				size);
			break;

		case P_SWEEP_RATE:
			if(value == m_fSweepRate)
				break;
				
			// set
			m_fSweepRate = value;
			m_tpSweepRateChanged = now;
			
			if(fOutput.destination != media_destination::null) {
				m_fThetaInc = calc_sweep_delta(
					fFormat.u.raw_audio,
					m_fSweepRate);
			}
			
			// broadcast
			BroadcastNewParameterValue(
				now,
				id,
				&m_fSweepRate,
				size);
			break;

		case P_DELAY:
			if(value == m_fDelay)
				break;
				
			// set
			m_fDelay = value;
			m_tpDelayChanged = now;
			
			if(fOutput.destination != media_destination::null) {
				m_fSweepBase = calc_sweep_base(
					fFormat.u.raw_audio,
					m_fDelay, m_fDepth);
			}
			
			// broadcast
			BroadcastNewParameterValue(
				now,
				id,
				&m_fDelay,
				size);
			break;

		case P_DEPTH:
			if(value == m_fDepth)
				break;
				
			// set
			m_fDepth = value;
			m_tpDepthChanged = now;
			
			if(fOutput.destination != media_destination::null) {
				m_fSweepBase = calc_sweep_base(
					fFormat.u.raw_audio,
					m_fDelay, m_fDepth);
				m_fSweepFactor = calc_sweep_factor(
					fFormat.u.raw_audio,
					m_fDepth);
			}
			
			// broadcast
			BroadcastNewParameterValue(
				now,
				id,
				&m_fDepth,
				size);
			break;
			
		case P_FEEDBACK:
			if(value == m_fFeedback)
				break;

			// set
			m_fFeedback = value;
			m_tpFeedbackChanged = now;
			// broadcast
			BroadcastNewParameterValue(
				now,
				id,
				&m_fFeedback,
				size);
			break;*/
	}
}

	
void
GenericAudioFilterNode::_HandleStartEvent(const media_timed_event* pEvent)
{
	PRINT(("GenericAudioFilterNode::handleStartEvent\n"));

	_StartFilter();	
}

		
void
GenericAudioFilterNode::_HandleStopEvent(const media_timed_event* pEvent)
{
	PRINT(("GenericAudioFilterNode::handleStopEvent\n"));
	
	_StopFilter();
}

		
void GenericAudioFilterNode::_IgnoreEvent(const media_timed_event* pEvent)
{
	PRINT(("GenericAudioFilterNode::ignoreEvent\n"));	
}
	

// #pragma mark -


// figure the preferred format: any fields left as wildcards
// are negotiable
void
GenericAudioFilterNode::_GetPreferredFormat(media_format& ioFormat)
{
	ASSERT(ioFormat.type == B_MEDIA_RAW_AUDIO);
	
	ioFormat.u.raw_audio = media_raw_audio_format::wildcard;
	ioFormat.u.raw_audio.channel_count = 2;
	ioFormat.u.raw_audio.format = media_raw_audio_format::B_AUDIO_FLOAT;
	
//	ioFormat.u.raw_audio.frame_rate = 44100.0;
//	ioFormat.u.raw_audio.buffer_size = 0x1000;
}


// test the given template format against a proposed format.
// specialize wildcards for fields where the template contains
// non-wildcard data; write required fields into proposed format
// if they mismatch.
// Returns B_OK if the proposed format doesn't conflict with the
// template, or B_MEDIA_BAD_FORMAT otherwise.
status_t
GenericAudioFilterNode::_ValidateProposedFormat(const media_format& preferredFormat,
	media_format& ioProposedFormat)
{
	char formatStr[256];
	PRINT(("GenericAudioFilterNode::_ValidateProposedFormat()\n"));

	ASSERT(preferredFormat.type == B_MEDIA_RAW_AUDIO);
	
	string_for_format(preferredFormat, formatStr, 255);
	PRINT(("\ttemplate format: %s\n", formatStr));

	string_for_format(ioProposedFormat, formatStr, 255);
	PRINT(("\tproposed format: %s\n", formatStr));
	
	status_t err = B_OK;
	
	if(ioProposedFormat.type != B_MEDIA_RAW_AUDIO) {
		// out of the ballpark
		ioProposedFormat = preferredFormat;
		return B_MEDIA_BAD_FORMAT;
	}

	// wildcard format
	media_raw_audio_format& wild = media_raw_audio_format::wildcard;
	// proposed format
	media_raw_audio_format& f = ioProposedFormat.u.raw_audio;
	// template format
	const media_raw_audio_format& pref = preferredFormat.u.raw_audio;
		
	if(pref.frame_rate != wild.frame_rate) {
		if(f.frame_rate != pref.frame_rate) {
			if(f.frame_rate != wild.frame_rate)
				err = B_MEDIA_BAD_FORMAT;
			f.frame_rate = pref.frame_rate;
		}
	}

	if(pref.channel_count != wild.channel_count) {
		if(f.channel_count != pref.channel_count) {
			if(f.channel_count != wild.channel_count)
				err = B_MEDIA_BAD_FORMAT;
			f.channel_count = pref.channel_count;
		}
	}
		
	if(pref.format != wild.format) {
		if(f.format != pref.format) {
			if(f.format != wild.format)
				err = B_MEDIA_BAD_FORMAT;
			f.format = pref.format;
		}
	}
		
	if(pref.byte_order != wild.byte_order) {
		if(f.byte_order != pref.byte_order) {
			if(f.byte_order != wild.byte_order)
				err = B_MEDIA_BAD_FORMAT;
			f.byte_order = pref.byte_order;
		}
	}
		
	if(pref.buffer_size != wild.buffer_size) {
		if(f.buffer_size != pref.buffer_size) {
			if(f.buffer_size != wild.buffer_size)
				err = B_MEDIA_BAD_FORMAT;
			f.buffer_size = pref.buffer_size;
		}
	}
	
	if(err != B_OK) {
		string_for_format(ioProposedFormat, formatStr, 255);
		PRINT((
			"\tformat conflict; suggesting:\n\tformat %s\n", formatStr));
	}
	
	return err;	
}


// fill in wildcards in the given format.
// (assumes the format passes _ValidateProposedFormat().)
void
GenericAudioFilterNode::_SpecializeOutputFormat(media_format& ioFormat)
{
	char formatStr[256];
	string_for_format(ioFormat, formatStr, 255);
	PRINT(("GenericAudioFilterNode::___SpecializeOutputFormat()\n"
		"\tinput format: %s\n", formatStr));
	
	ASSERT(ioFormat.type == B_MEDIA_RAW_AUDIO);
	
	// carpal_tunnel_paranoia
	media_raw_audio_format& f = ioFormat.u.raw_audio;
	media_raw_audio_format& w = media_raw_audio_format::wildcard;
	
	if(f.frame_rate == w.frame_rate)
		f.frame_rate = 44100.0;
		
	if(f.channel_count == w.channel_count) {
		//+++++ tweaked 15sep99
		//if(fInput.source != media_source::null)
			f.channel_count = fInput.format.u.raw_audio.channel_count;
		//else
		//	f.channel_count = 1; //whyyyy ALEX
	}
	if(f.format == w.format)
		f.format = media_raw_audio_format::B_AUDIO_FLOAT;
	if(f.byte_order == w.format)
		f.byte_order = (B_HOST_IS_BENDIAN) ? B_MEDIA_BIG_ENDIAN : B_MEDIA_LITTLE_ENDIAN;
	if(f.buffer_size == w.buffer_size)
		f.buffer_size = 2048;

	string_for_format(ioFormat, formatStr, 255);
	PRINT(("\toutput format: %s\n", formatStr));
}


// set parameters to their default settings
void
GenericAudioFilterNode::_InitParameterValues()
{
	/*fFlangerMixRatio = 0.5;
	m_tpMixRatioChanged = 0LL;
	
	m_fSweepRate = 0.1;
	m_tpSweepRateChanged = 0LL;
	
	m_fDelay = 10.0;
	m_tpDelayChanged = 0LL;

	m_fDepth = 25.0;
	m_tpDepthChanged = 0LL;

	m_fFeedback = 0.1;
	m_tpFeedbackChanged = 0LL;*/
}

	
// create and register a parameter web
void
GenericAudioFilterNode::_InitParameterWeb()
{
	BParameterWeb* pWeb = new BParameterWeb();
	BParameterGroup* pTopGroup = pWeb->MakeGroup("GenericAudioFilterNode Parameters");

	BNullParameter* label;
	BContinuousParameter* value;
	BParameterGroup* g;
	
	float s_fMaxDelay = 1.0; //temp to build
	
	// mix ratio
	g = pTopGroup->MakeGroup("Mix Ratio");
	label = g->MakeNullParameter(
		P_MIX_RATIO_LABEL,
		B_MEDIA_NO_TYPE,
		"Mix Ratio",
		B_GENERIC);
	
	value = g->MakeContinuousParameter(
		P_MIX_RATIO,
		B_MEDIA_NO_TYPE,
		"",
		B_GAIN, "", 0.0, 1.0, 0.05);
	label->AddOutput(value);
	value->AddInput(label);

	// sweep rate
	g = pTopGroup->MakeGroup("Sweep Rate");
	label = g->MakeNullParameter(
		P_SWEEP_RATE_LABEL,
		B_MEDIA_NO_TYPE,
		"Sweep Rate",
		B_GENERIC);
	
	value = g->MakeContinuousParameter(
		P_SWEEP_RATE,
		B_MEDIA_NO_TYPE,
		"",
		B_GAIN, "Hz", 0.01, 10.0, 0.01);
	label->AddOutput(value);
	value->AddInput(label);

	// sweep range: minimum delay
	g = pTopGroup->MakeGroup("Delay");
	label = g->MakeNullParameter(
		P_DELAY_LABEL,
		B_MEDIA_NO_TYPE,
		"Delay",
		B_GENERIC);
	
	value = g->MakeContinuousParameter(
		P_DELAY,
		B_MEDIA_NO_TYPE,
		"",
		B_GAIN, "msec", 0.1, s_fMaxDelay/2.0, 0.1);
	label->AddOutput(value);
	value->AddInput(label);

	// sweep range: maximum
	g = pTopGroup->MakeGroup("Depth");
	label = g->MakeNullParameter(
		P_DEPTH_LABEL,
		B_MEDIA_NO_TYPE,
		"Depth",
		B_GENERIC);
	
	value = g->MakeContinuousParameter(
		P_DEPTH,
		B_MEDIA_NO_TYPE,
		"",
		B_GAIN, "msec", 1.0, s_fMaxDelay/4.0, 0.1);
	label->AddOutput(value);
	value->AddInput(label);

	// feedback
	g = pTopGroup->MakeGroup("Feedback");
	label = g->MakeNullParameter(
		P_FEEDBACK_LABEL,
		B_MEDIA_NO_TYPE,
		"Feedback",
		B_GENERIC);
	
	value = g->MakeContinuousParameter(
		P_FEEDBACK,
		B_MEDIA_NO_TYPE,
		"",
		B_GAIN, "", 0.0, 1.0, 0.01);
	label->AddOutput(value);
	value->AddInput(label);

	// * Install parameter web
	SetParameterWeb(pWeb);
}


void
GenericAudioFilterNode::_InitFilter()
{
	PRINT(("GenericAudioFilterNode::_InitFilter()\n"));
	ASSERT(fFormat.u.raw_audio.format != media_raw_audio_format::wildcard.format);
	
	fFaustDSP->init(44100);
	
	fDevNumInChans = fFaustDSP->getNumInputs(); //2;
	fDevNumOutChans = fFaustDSP->getNumOutputs(); //2; 
	AllocChannels(1024, fDevNumInChans, fDevNumOutChans);
}


void
GenericAudioFilterNode::_StartFilter()
{
	PRINT(("GenericAudioFilterNode::_StartFilter()\n"));
}


void
GenericAudioFilterNode::_StopFilter()
{
	PRINT(("GenericAudioFilterNode::stopFilter()\n"));
}


// figure processing latency by doing 'dry runs' of _FilterBuffer()
bigtime_t
GenericAudioFilterNode::_CalcProcessingLatency()
{
	PRINT(("GenericAudioFilterNode::_CalcProcessingLatency()\n"));
	
	if(fOutput.destination == media_destination::null) {
		PRINT(("\tNot connected.\n"));
		return 0LL;
	}
	
	// allocate a temporary buffer group
	BBufferGroup* pTestGroup = new BBufferGroup(
		fOutput.format.u.raw_audio.buffer_size, 1);
	
	// fetch a buffer
	BBuffer* pBuffer = pTestGroup->RequestBuffer(
		fOutput.format.u.raw_audio.buffer_size);
	ASSERT(pBuffer);
	
	pBuffer->Header()->type = B_MEDIA_RAW_AUDIO;
	pBuffer->Header()->size_used = fOutput.format.u.raw_audio.buffer_size;
	
	// run the test
	bigtime_t preTest = system_time();
	_FilterBuffer(pBuffer);
	bigtime_t elapsed = system_time() - preTest;
	
	// clean up
	pBuffer->Recycle();
	delete pTestGroup;

	// reset filter state
	_InitFilter();

	return elapsed;
}


void
GenericAudioFilterNode::AllocChannels(int size, int numInChan, int numOutChan) 
{	
	ASSERT(numInChan < 256);
	ASSERT(numOutChan < 256);
	
	
	for (int i = 0; i < numInChan; i++) {
		fInChannel[i] = (float*) calloc (size, sizeof(float));
		for (int j = 0; j < size; j++) {
			fInChannel[i][j] = 0.0;
		}
	}
	
	for (int i = 0; i < numOutChan; i++) {
		fOutChannel[i] = (float*) calloc (size, sizeof(float));
		for (int j = 0; j < size; j++) {
			fOutChannel[i][j] = 0.0;
		}
	}
}


void
GenericAudioFilterNode::_FilterBuffer(BBuffer* buffer)
{	
	media_raw_audio_format format = fFormat.u.raw_audio;
	
	uint32 frameCount = buffer->Header()->size_used / bytes_per_frame(format);
	
	float* fInputBuffer = (float*) buffer->Data();
	float* fOutputBuffer = (float*) buffer->Data();
		
	// split input samples
	for (unsigned long frame = 0; frame < frameCount; frame++) {
		for (int c = 0; c < fDevNumInChans; c++) {
			fInChannel[c][frame] = fInputBuffer[c + frame * fDevNumInChans];
		}
	}
	
	// process samples
	fFaustDSP->compute(frameCount, fInChannel, fOutChannel);

	// merge output samples
	for (unsigned long frame = 0; frame < frameCount; frame++) {
		for (int c = 0; c < fDevNumOutChans; c++) {
			fOutputBuffer[c + frame * fDevNumOutChans] = fOutChannel[c][frame];
		}
	}	
}
