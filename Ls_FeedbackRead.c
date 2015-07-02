// Read frame from buffer written by Ls_FeedbackWrite
// lewis@lewissaunders.com

#include "half.h"
#ifdef __APPLE__
	#include "/usr/discreet/presets/2016/sparks/spark.h"
#else
	#include "/usr/discreet/flame_2013.0.4/sparks/spark.h"
#endif

int getbuf(int n, SparkMemBufStruct *b) {
	if(!sparkMemGetBuffer(n, b)) {
		printf("__FILE__: sparkMemGetBuffer() failed: %d\n", n);
		return(0);
	}
	if(!(b->BufState & MEMBUF_LOCKED)) {
		printf("__FILE__: buffer %d not locked\n", n);
		return(0);
	}
	return(1);
}

unsigned long *SparkProcess(SparkInfoStruct si) {
	SparkMemBufStruct result, front;
	if(!getbuf(1, &result)) return(NULL);
	if(!getbuf(2, &front)) return(NULL);
	
	return(result.Buffer);
}

unsigned int SparkInitialise(SparkInfoStruct si) {
	return(SPARK_MODULE);
}

void SparkUnInitialise(SparkInfoStruct sparkInfo) {
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
