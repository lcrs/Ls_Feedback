// Copy input frame to a buffer and pass the address to Ls_FeedbackRead
// lewis@lewissaunders.com

#include <sys/errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "/usr/discreet/presets/2016/sparks/spark.h"

void *feedbackbuffer = NULL;
int shmfd;
void *shmptr = NULL;
float uniqueseed = 0.0;

SparkFloatStruct SparkFloat7 = {
	0.0,						// Initial
	-INFINITY,					// Min
	+INFINITY,					// Max
	0.001,						// Increment
	0,							// Flags
	(char *) "Unique seed %f",	// Name
	NULL						// Callback
};

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

	if(feedbackbuffer) {
		memcpy(feedbackbuffer, front.Buffer, front.BufSize);
		*(void **)shmptr = feedbackbuffer;
	} else {
		printf("Ls_FeedbackWrite: SparkProcess() but no feedbackbuffer!\n");
		*(void **)shmptr = NULL;
	}

	// Increment unqiue seed
	// This is an attempt to defeat Flame's caching, which presumably
	// relies on hashing this value
	uniqueseed += 0.01;
	SparkFloat7.Value = uniqueseed;
	sparkSetCurveKey(SPARK_UI_CONTROL, 7, si.FrameNo, uniqueseed);
	sparkControlUpdate(7);

	sparkCopyBuffer(front.Buffer, result.Buffer);
	return(result.Buffer);
}

unsigned int SparkInitialise(SparkInfoStruct si) {
	feedbackbuffer = (void *) malloc(si.FrameBytes);

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
	close(shmfd);
	shm_unlink("Ls_Feedback");
	munmap(shmptr, 8);
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
