// GenericAudioFilterNode.h
// * PURPOSE
// - implements a basic audio filter
// - eventually abstract -> 'SimpleAudioFilter'?
//
//
// * HISTORY
//   e.moon		15jun99		Begun

#ifndef __GenericAudioFilterNode_H__
#define __GenericAudioFilterNode_H__

#include <BufferProducer.h>
#include <BufferConsumer.h>
#include <Controllable.h>
#include <MediaEventLooper.h>

//test
#include "FaustDSP.h"


class BBufferGroup;
class AudioBuffer;

class GenericAudioFilterNode :
	public		BBufferConsumer,
	public		BBufferProducer,
	public		BControllable,
	public		BMediaEventLooper {
	
public:
	virtual		~GenericAudioFilterNode();
				GenericAudioFilterNode(FaustDSP* faustDSP);
	
public:					// *** BMediaNode

	virtual status_t 		HandleMessage(int32 code, const void* pData, size_t size);
	virtual BMediaAddOn* 	AddOn(int32* outInternalID) const;
	virtual void 			SetRunMode(run_mode mode);
	
protected:				// *** BMediaEventLooper

	virtual void HandleEvent(
		const media_timed_event* pEvent,
		bigtime_t howLate,
		bool realTimeEvent=false);

protected:
	// "The Media Server calls this hook function after the node has
	//  been registered.  This is derived from BMediaNode; BMediaEventLooper
	//  implements it to call Run() automatically when the node is registered;
	//  if you implement NodeRegistered() you should call through to
	//  BMediaEventLooper::NodeRegistered() after you've done your custom 
	//  operations."

	virtual void NodeRegistered();
	
	// "Augment OfflineTime() to compute the node's current time; it's called
	//  by the Media Kit when it's in offline mode. Update any appropriate
	//  internal information as well, then call through to the BMediaEventLooper
	//  implementation."

	virtual bigtime_t OfflineTime();

public:					// *** BBufferConsumer

	virtual status_t AcceptFormat(
		const media_destination& destination,
		media_format* pioFormat);
	
	// "If you're writing a node, and receive a buffer with the B_SMALL_BUFFER
	//  flag set, you must recycle the buffer before returning."	

	virtual void BufferReceived(
		BBuffer* pBuffer);
	
	// * make sure to fill in poInput->format with the contents of
	//   pFormat; as of R4.5 the Media Kit passes poInput->format to
	//   the producer in BBufferProducer::Connect().

	virtual status_t Connected(
		const media_source& source,
		const media_destination& destination,
		const media_format& format,
		media_input* poInput);

	virtual void Disconnected(
		const media_source& source,
		const media_destination& destination);
		
	virtual void DisposeInputCookie(
		int32 cookie);
	
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

	virtual status_t FormatChanged(
		const media_source& source,
		const media_destination& destination,
		int32 changeTag,
		const media_format& newFormat);
		
	virtual status_t GetLatencyFor(
		const media_destination& destination,
		bigtime_t* poLatency,
		media_node_id* poTimeSource);
		
	virtual status_t GetNextInput(
		int32* pioCookie,
		media_input* poInput);

	virtual void ProducerDataStatus(
		const media_destination& destination,
		int32 status,
		bigtime_t tpWhen);
	
	// "This function is provided to aid in supporting media formats in which the
	//  outer encapsulation layer doesn't supply timing information. Producers will
	//  tag the buffers they generate with seek tags; these tags can be used to
	//  locate key frames in the media data."

	virtual status_t SeekTagRequested(
		const media_destination& destination,
		bigtime_t targetTime,
		uint32 flags,
		media_seek_tag* poSeekTag,
		bigtime_t* poTaggedTime,
		uint32* poFlags);
	
public:					// *** BBufferProducer

	// "When a consumer calls BBufferConsumer::RequestAdditionalBuffer(), this
	//  function is called as a result. Its job is to call SendBuffer() to
	//  immediately send the next buffer to the consumer. The previousBufferID,
	//  previousTime, and previousTag arguments identify the last buffer the
	//  consumer received. Your node should respond by sending the next buffer
	//  after the one described. 
	//
	//  The previousTag may be NULL.
	//  Return B_OK if all is well; otherwise return an appropriate error code."
	virtual void AdditionalBufferRequested(
		const media_source& source,
		media_buffer_id previousBufferID,
		bigtime_t previousTime,
		const media_seek_tag* pPreviousTag); //nyi
		
	virtual void Connect(
		status_t status,
		const media_source& source,
		const media_destination& destination,
		const media_format& format,
		char* pioName); //nyi
		
	virtual void Disconnect(
		const media_source& source,
		const media_destination& destination); //nyi
		
	virtual status_t DisposeOutputCookie(
		int32 cookie); //nyi
		
	virtual void EnableOutput(
		const media_source& source,
		bool enabled,
		int32* _deprecated_); //nyi
		
	virtual status_t FormatChangeRequested(
		const media_source& source,
		const media_destination& destination,
		media_format* pioFormat,
		int32* _deprecated_); //nyi
		
	virtual status_t FormatProposal(
		const media_source& source,
		media_format* pioFormat); //nyi
		
	virtual status_t FormatSuggestionRequested(
		media_type type,
		int32 quality,
		media_format* poFormat); //nyi
		
	virtual status_t GetLatency(
		bigtime_t* poLatency); //nyi
		
	virtual status_t GetNextOutput(
		int32* pioCookie,
		media_output* poOutput); //nyi
	
	virtual void LatencyChanged(
		const media_source& source,
		const media_destination& destination,
		bigtime_t newLatency,
		uint32 flags); //nyi

	virtual void LateNoticeReceived(
		const media_source& source,
		bigtime_t howLate,
		bigtime_t tpWhen); //nyi
		
	virtual status_t PrepareToConnect(
		const media_source& source,
		const media_destination& destination,
		media_format* pioFormat,
		media_source* poSource,
		char* poName); //nyi
		
	virtual status_t SetBufferGroup(
		const media_source& source,
		BBufferGroup* pGroup); //nyi
	
	virtual status_t SetPlayRate(
		int32 numerator,
		int32 denominator); //nyi
	
	virtual status_t VideoClippingChanged(
		const media_source& source,
		int16 numShorts,
		int16* pClipData,
		const media_video_display_info& display,
		int32* poFromChangeTag); //nyi

public:					// *** BControllable

	virtual status_t GetParameterValue(
		int32 id,
		bigtime_t* poLastChangeTime,
		void* poValue,
		size_t* pioSize); //nyi
		
	virtual void SetParameterValue(
		int32 id,
		bigtime_t changeTime,
		const void* pValue,
		size_t size); //nyi

	
protected:				// HandleEvent() impl.
	void	_HandleParameterEvent(const media_timed_event* pEvent);		
	void	_HandleStartEvent(const media_timed_event* pEvent);		
	void	_HandleStopEvent(const media_timed_event* pEvent);		
	void	_IgnoreEvent(const media_timed_event* pEvent);

protected:				// *** internal operations

	// figure the preferred format: any fields left as wildcards
	// are negotiable
	virtual void _GetPreferredFormat(media_format& ioFormat);
		
	// test the given template format against a proposed format.
	// specialize wildcards for fields where the template contains
	// non-wildcard data; write required fields into proposed format
	// if they mismatch.
	// Returns B_OK if the proposed format doesn't conflict with the
	// template, or B_MEDIA_BAD_FORMAT otherwise.

	status_t _ValidateProposedFormat(const media_format& preferredFormat,
		media_format& ioProposedFormat);
		
	// fill in wildcards in the given format.
	// (assumes the format passes __ValidateProposedFormat().)
	void _SpecializeOutputFormat(media_format& ioFormat);
		
	// set parameters to their default settings
	virtual void _InitParameterValues();
	
	// create and register a parameter web
	virtual void _InitParameterWeb();
	
	// construct delay line if necessary, reset filter state
	virtual void _InitFilter();
	
	virtual void _StartFilter();
	virtual void _StopFilter();

	// figure processing latency by doing 'dry runs' of _FilterBuffer()
	virtual bigtime_t _CalcProcessingLatency();
	
	// filter buffer data in place	
	virtual void _FilterBuffer(BBuffer* buffer);
		
private:					// *** connection/format members

	// The 'template' format
	// +++++ init in NodeRegistered()
	media_format			fPreferredFormat;

	// The current input/output format (this filter doesn't do any
	// on-the-fly conversion.)  Any fields that are not wildcards
	// are mandatory; the first connection (input or output) decides
	// the node's format.  If both input and output are disconnected,
	// fFormat.u.raw_audio should revert to media_raw_audio_format::wildcard.	
	media_format			fFormat;
	
	// Connections & associated state variables	
	media_input				fInput;

	media_output			fOutput;
	bool					fOutputEnabled;

// [16jun99] buffers are generated by the upstream producer; this
//           node processes them in-place and forwards them downstream.
//	
//	// The outbound buffer group	
//	BBufferGroup*			m_pBufferGroup;

	// Time required by downstream consumer(s) to properly deliver a buffer
	bigtime_t				fDownstreamLatency;

	// Worst-case time needed to fill a buffer
	bigtime_t				fProcessingLatency;

private:	
	static const char* const		kNodeName;
	
	FaustDSP*		fFaustDSP;
	float*			fInChannel[256];
	float*			fOutChannel[256];
	int				fDevNumInChans;
	int				fDevNumOutChans;
	void			AllocChannels(int size, int numInChan, int numOutChan);
	
public:
	static float fMarcel;
	
};

#endif /*__GenericAudioFilterNode_H__*/
