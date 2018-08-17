#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "jni.h"
#include "hdfs.h"


struct work_param {
  hdfsFS fs;
  int worker_id;
  long num_iters;
};

static void *worker(void* arg) {
  work_param* params = (work_param*)arg;
  hdfsFS fs = params->fs;
  fprintf(stderr, "Hello %i\n", params->worker_id);

  for (long i = 0; i < params->num_iters; ++i) {
    char writePath[128];
    sprintf(writePath, "/tmp/testfile%i.txt", params->worker_id);
    hdfsFile writeFile = hdfsOpenFile(fs, writePath, O_WRONLY|O_CREAT, 0, 0, 0);
    if(!writeFile) {
      fprintf(stderr, "Failed to open %s for writing!\n", writePath);
      exit(-1);
    }
    const char* buffer = "Hello, World!";
    tSize num_written_bytes = hdfsWrite(fs, writeFile, (void*)buffer, strlen(buffer)+1);
    if (hdfsFlush(fs, writeFile)) {
      fprintf(stderr, "Failed to 'flush' %s\n", writePath); 
      exit(-1);
    }
    hdfsCloseFile(fs, writeFile);
    if (i % 1000 == 0) {
      fprintf(stderr, "T%i: Wrote %li/%li files!\n", params->worker_id, i + 1, params->num_iters);
    }
  }
  fprintf(stderr, "T%i: Wrote %li files!\n", params->worker_id, params->num_iters);
}

int
main(int argc, char **argv)
{
  hdfsFS fs = hdfsConnect("default", 0);
  if (argc != 3) {
    fprintf(stderr, "Wrong args\n");
    return 1;
  }
  int num_workers = atoi(argv[1]);
  long num_iters = atol(argv[2]);
  work_param params[num_workers];
  pthread_t threads[num_workers];
  pthread_attr_t attrs[num_workers];
  for (int i = 0; i < num_workers; ++i) {
    params[i].fs = fs;
    params[i].worker_id = i;
    params[i].num_iters = num_iters;
    if (pthread_attr_init(&attrs[i]) != 0) {
       fprintf(stderr, "pthread_attr_init\n");
       return 1;
    }
    if (pthread_create(&threads[i], &attrs[i], &worker, &params[i]) != 0) {
      fprintf(stderr, "Could not create thread %i\n", i);
      return 1;
    }
  }
  for (int i = 0; i < num_workers; ++i) {
    void* res;
    if (pthread_join(threads[i], &res) != 0) {
      fprintf(stderr, "Could not join thread %i\n", i);
      return 1;
    }
  }
  return 0;
}
