// Read frame from buffer written by Ls_FeedbackWrite
// lewis@lewissaunders.com

#include "half.h"
#include <sys/mman.h>
#include <fcntl.h>
#ifdef __APPLE__
	#include "/usr/discreet/presets/2016/sparks/spark.h"
#else
	#include "/usr/discreet/flame_2013.0.4/sparks/spark.h"
#endif

half *feedbackbuffer = NULL;
int shmfd;
void *shmptr = NULL;

int getbuf(int n, SparkMemBufStruct *b) {
	if(!sparkMemGetBuffer(n, b)) {
		printf("Ls_FeedbackRead: sparkMemGetBuffer() failed: %d\n", n);
		return(0);
	}
	if(!(b->BufState & MEMBUF_LOCKED)) {
		printf("Ls_FeedbackRead: buffer %d not locked\n", n);
		return(0);
	}
	return(1);
}

unsigned long *SparkProcess(SparkInfoStruct si) {
	SparkMemBufStruct result, front;

	printf("Ls_FeedbackRead: SparkProcess()\n");

	if(si.FrameNo == 0) {
		// Pass through front input
		if(!getbuf(1, &result)) return(NULL);
		if(!getbuf(2, &front)) return(NULL);
		printf("Ls_FeedbackRead: SparkProcess() passing front through\n");
		sparkCopyBuffer(front.Buffer, result.Buffer);
		return(result.Buffer);
	}

	if(!getbuf(1, &result)) return(NULL);

	// Read shared pointer
	shmfd = shm_open("Ls_Feedback", O_RDONLY, 0700);
	if(shmfd == -1) {
		printf("Ls_FeedbackRead: shm_open() failed: %d\n", errno);
	}
	shmptr = mmap(0, 8, PROT_READ, MAP_SHARED, shmfd, 0);
	if(shmptr == MAP_FAILED) {
		printf("Ls_FeedbackWrite: mmap() failed: %d\n", errno);
		shmptr = NULL;
	}
	if(shmptr) {
		feedbackbuffer = *(half **)shmptr;
		if(feedbackbuffer) {
			printf("Ls_FeedbackRead: copying from %p\n", feedbackbuffer);
			memcpy(result.Buffer, feedbackbuffer, result.BufSize);
			printf("Ls_FeedbackRead: ...copied\n");
		}
	}

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
