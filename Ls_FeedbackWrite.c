// Write input frame to a buffer and pass the address to Ls_FeedbackRead
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
	SparkMemBufStruct front, result;
	if(!getbuf(1, &result)) return(NULL);
	if(!getbuf(2, &front)) return(NULL);

	printf("Ls_FeedbackWrite: SparkProcess()\n");

	if(feedbackbuffer) {
		memcpy(feedbackbuffer, front.Buffer, front.BufSize);
		printf("Ls_FeedbackWrite: SparkProcess() did copy to %p\n", feedbackbuffer);
		*(void **)shmptr = feedbackbuffer;
	} else {
		printf("Ls_FeedbackWrite: SparkProcess() but no feedbackbuffer!\n");
		*(void **)shmptr = NULL;
	}

	sparkCopyBuffer(front.Buffer, result.Buffer);
	return(result.Buffer);
}

unsigned int SparkInitialise(SparkInfoStruct si) {
	feedbackbuffer = (half *) malloc(si.FrameBytes);
	printf("Ls_FeedbackWrite: malloc(): %d bytes at %p\n", si.FrameBytes, feedbackbuffer);

	shmfd = shm_open("Ls_Feedback", O_CREAT | O_RDWR, 0700);
	if(shmfd == -1) {
		printf("Ls_FeedbackWrite: shm_open() failed: %d\n", errno);
	}
	ftruncate(shmfd, 8);
	shmptr = mmap(0, 8, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	if(shmptr == MAP_FAILED) {
		printf("Ls_FeedbackWrite: mmap() failed: %d\n", errno);
		shmptr = NULL;
	}
	*(void **)shmptr = NULL;

	return(SPARK_MODULE);
}

void SparkUnInitialise(SparkInfoStruct sparkInfo) {
	*(void **)shmptr = NULL;
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
