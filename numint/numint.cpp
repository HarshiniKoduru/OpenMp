#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif


/*Numerical integrate function passes command line arguments to provide values for scehduling policy type, a,b,n,threads number,granularity*/

float numerical_integrate(char* argv[]) {
  int functionid = atoi(argv[1]);
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  unsigned long n = atol(argv[4]);
  uint32_t intensity = atoi(argv[5]);
  int thrds_number = atoi(argv[6]);
  std::string sched_type = argv[7];
  int granularity = atoi(argv[8]);
  float multiplier = (b - a) / (float)n;
  float sum_result = 0.0;

  omp_set_num_threads(thrds_number);

  if (sched_type.compare("static") == 0) {
    if (granularity > 0) {
      float x = 0.0;
#pragma omp parallel for schedule(static, granularity) reduction(+ : sum_result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            sum_result = sum_result + f1(x, intensity) * multiplier;
            break;
          case 2:
            sum_result = sum_result + f2(x, intensity) * multiplier;
            break;
          case 3:
            sum_result = sum_result + f3(x, intensity) * multiplier;
            break;
          case 4:
            sum_result = sum_result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }

    } else {
      float x = 0.0;
#pragma omp parallel for schedule(static) reduction(+ : sum_result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            sum_result = sum_result + f1(x, intensity) * multiplier;
            break;
          case 2:
            sum_result = sum_result + f2(x, intensity) * multiplier;
            break;
          case 3:
            sum_result = sum_result + f3(x, intensity) * multiplier;
            break;
          case 4:
            sum_result = sum_result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }
    }
  } else if (sched_type.compare("dynamic") == 0) {
    if (granularity > 0) {
      float x = 0.0;
#pragma omp parallel for schedule(dynamic, granularity) reduction(+ : sum_result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            sum_result = sum_result + f1(x, intensity) * multiplier;
            break;
          case 2:
            sum_result = sum_result + f2(x, intensity) * multiplier;
            break;
          case 3:
            sum_result = sum_result + f3(x, intensity) * multiplier;
            break;
          case 4:
            sum_result = sum_result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }

    } else {
      float x = 0.0;
#pragma omp parallel for schedule(dynamic) reduction(+ : sum_result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            sum_result = sum_result + f1(x, intensity) * multiplier;
            break;
          case 2:
            sum_result = sum_result + f2(x, intensity) * multiplier;
            break;
          case 3:
            sum_result = sum_result + f3(x, intensity) * multiplier;
            break;
          case 4:
            sum_result = sum_result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }
    }
  } else if (sched_type.compare("guided") == 0) {
    if (granularity > 0) {
      float x = 0.0;
#pragma omp parallel for schedule(guided, granularity) reduction(+ : sum_result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            sum_result = sum_result + f1(x, intensity) * multiplier;
            break;
          case 2:
            sum_result = sum_result + f2(x, intensity) * multiplier;
            break;
          case 3:
            sum_result = sum_result + f3(x, intensity) * multiplier;
            break;
          case 4:
            sum_result = sum_result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }

    } else {
      float x = 0.0;
#pragma omp parallel for schedule(guided) reduction(+ : sum_result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            sum_result = sum_result + f1(x, intensity) * multiplier;
            break;
          case 2:
            sum_result = sum_result + f2(x, intensity) * multiplier;
            break;
          case 3:
            sum_result = sum_result + f3(x, intensity) * multiplier;
            break;
          case 4:
            sum_result = sum_result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }
    }
  }
  return sum_result;
}

/*Main function calculates the execution time*/

int main(int argc, char* argv[]) {
// forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open(argv[0], O_RDONLY);
    if (fd != -1) {
      close(fd);
    } else {
      std::cerr << "something is amiss" << std::endl;
    }
  }

  if (argc < 9) {
    std::cerr << "Usage: " << argv[0]
              << " <functionid> <a> <b> <n> <intensity> <nbthreads> "
                 "<scheduling> <granularity>"
              << std::endl;
    return -1;
  }

  std::chrono::time_point<std::chrono::system_clock> start =
      std::chrono::system_clock::now();
  float sum_result = numerical_integrate(argv);
  std::chrono::time_point<std::chrono::system_clock> end =
      std::chrono::system_clock::now();
  std::chrono::duration<double> total_time = end - start;

  std::cout << sum_result << std::endl;

  std::cerr << total_time.count() << std::endl;

  return 0;
}

  
