// Write input frame to a buffer and pass the address to Ls_FeedbackRead
// lewis@lewissaunders.com

#include "half.h"
#ifdef __APPLE__
	#include "/usr/discreet/presets/2016/sparks/spark.h"
#else
	#include "/usr/discreet/flame_2013.0.4/sparks/spark.h"
#endif

half *feedbackbuffer = NULL;

int getbuf(int n, SparkMemBufStruct *b) {
	if(!sparkMemGetBuffer(n, b)) {
		printf("Ls_FeedbackWrite: sparkMemGetBuffer() failed: %d\n", n);
		return(0);
	}
	if(!(b->BufState & MEMBUF_LOCKED)) {
		printf("Ls_FeedbackWrite: buffer %d not locked\n", n);
		return(0);
	}
	return(1);
}

unsigned long *SparkProcess(SparkInfoStruct si) {
	SparkMemBufStruct front;
	if(!getbuf(2, &front)) return(NULL);

	printf("Ls_FeedbackWrite: SparkProcess()\n");

	if(feedbackbuffer) {
		memcpy(feedbackbuffer, front.Buffer, front.BufSize);
		printf("Ls_FeedbackWrite: SparkProcess() did copy\n");
		// Write pointer to socket
	} else {
		printf("Ls_FeedbackWrite: SparkProcess() but no feedbackbuffer!\n");
	}
	
	return(front.Buffer);
}

unsigned int SparkInitialise(SparkInfoStruct si) {
	feedbackbuffer = (half *) malloc(si.FrameBytes);
	printf("Ls_FeedbackWrite: malloc(): %d bytes at %p\n", si.FrameBytes, feedbackbuffer);
	return(SPARK_MODULE);
}

void SparkUnInitialise(SparkInfoStruct sparkInfo) {
	free(feedbackbuffer);
	printf("Ls_FeedbackWrite: free(): %p\n", feedbackbuffer);
}

int SparkIsInputFormatSupported(SparkPixelFormat fmt) {
	switch(fmt) {
		case SPARKBUF_RGB_48_3x16_FP:
			return(1);
		default:
			return(0);
	}
}

int SparkClips(void) {
	return(1);
}

void SparkMemoryTempBuffers(void) { /* Must be defined to keep Batch happy */ }
