// UCSD CSE237A - WI22
// Important! TODO: You need to modify this file, but WILL NOT SUBMIT this file.
// You can characterize the given workloads by chainging this file,
// and WILL SUBMIT the analysis report based on the characterization results.
// For more details, please see the instructions in the class website.

// main.c: Main file to characterize different workloads

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "cpufreq.h"
#include "workload_util.h"
#include "workload.h"

// power consumption levels
#define PowerType unsigned long long
const PowerType power_max = 1200;
const PowerType power_min = 500;
const char filename[] = "result.txt";
FILE *fp;

// The function can be called after finishing workload(s)
void report_measurement(int freq, PerfData* perf_msmts, PowerType power) {
    int core;
    for (core = 0; core < MAX_CPU_IN_RPI4; ++core) {
        PerfData* pf = &perf_msmts[core]; 
        if (pf->is_used == 0)
            continue;

        TimeType time_estimated = (TimeType)pf->cc/(TimeType)(freq/1000);
        fprintf(fp, "[Core %d] Execution Time (us): %lld\r\n", core, time_estimated);
        fprintf(fp, "[Core %d] Energy (mJ): %lld\r\n", core, time_estimated*power/1000000);

        fprintf(fp, "[Core %d] Cycle Count: %u\r\n", core, pf->cc);
        fprintf(fp, "[Core %d] Instructions: %u\r\n", core, pf->insts);

        fprintf(fp, "[Core %d] L1 Cache Accesses: %u\r\n", core, pf->l1access);
        fprintf(fp, "[Core %d] L1 Cache Misses: %u\r\n", core, pf->l1miss);
        if (pf->l1access != 0)
            fprintf(fp, "[Core %d] L1 Miss Ratio: %lf\r\n",
                    core, (double)pf->l1miss/(double)pf->l1access);

        fprintf(fp, "[Core %d] LLC Accesses: %u\r\n", core, pf->llaccess);
        fprintf(fp, "[Core %d] LLC Misses: %u\r\n", core, pf->llcmiss);
        if (pf->llaccess != 0)
            fprintf(fp, "[Core %d] LLC Miss Ratio: %lf\r\n",
                    core, (double)pf->llcmiss/(double)pf->llaccess);

        fprintf(fp, "[Core %d] Branch Miss-Predictions: %u\r\n", core, pf->branchmisspred);
    }
}

// run test under different task configuration and report the measurement
// two tests under max and min freq
void run_and_measure() {
    // 1. Set CPU frequency - Max Freq
    set_by_max_freq(); 
    int freq = get_cur_freq();
    // 2. Run workload
    fprintf(fp, "Characterization starts. Max Frequency.\r\n");
    PerfData perf_msmts[MAX_CPU_IN_RPI4];
    TimeType start_time = get_current_time_us();
    run_workloads(perf_msmts);

    // 3. Here, we get elapsed time and performance counters.
    fprintf(fp, "Total Execution time (us): %lld at %d\r\n",
            get_current_time_us() - start_time, get_cur_freq());
    report_measurement(freq, perf_msmts, power_max);

    // 1. Set CPU frequency - Min Freq
    set_by_min_freq(); 
    freq = get_cur_freq();
    // 2. Run workload
    fprintf(fp, "Characterization starts. Min Frequency.\r\n");
    start_time = get_current_time_us();
    run_workloads(perf_msmts);

    // 3. Here, we get elapsed time and performance counters.
    fprintf(fp, "Total Execution time (us): %lld at %d\r\n",
            get_current_time_us() - start_time, get_cur_freq());
    report_measurement(freq, perf_msmts, power_min);
}

int main(int argc, char *argv[]) {
    fp = fopen(filename, "w+");
    if (!fp) {
        printf("open file error!");
        exit(1);
    }
    
    time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    printf("Initialization.\n");

    /*******************************************************************
    * Start
    *******************************************************************/
    set_userspace_governor();
   
    //1. w1, c0
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 1 on Core 0 started\n");
    printf("WorkLoad 1 on Core 0 started.\n");

    register_workload(0, workload1_init, workload1_body, workload1_exit);
    run_and_measure();
    unregister_workload_all();

    fprintf(fp, "WorkLoad 1 on Core 0 is completed\n\n");
    printf("WorkLoad 1 on Core 0 is completed.\n\n");

    //2. w2, c0
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 2 on Core 0 started\n");
    printf("WorkLoad 2 on Core 0 started.\n");

    register_workload(0, workload2_init, workload2_body, workload2_exit);
    run_and_measure();
    unregister_workload_all();
    
    fprintf(fp, "WorkLoad 2 on Core 0 is completed\n\n");
    printf("WorkLoad 2 on Core 0 is completed.\n\n");

    //3. w3, c0
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 3 on Core 0 started\n");
    printf("WorkLoad 3 on Core 0 started.\n");

    register_workload(0, workload3_init, workload3_body, workload3_exit);
    run_and_measure();
    unregister_workload_all();
    
    fprintf(fp, "WorkLoad 3 on Core 0 is completed\r\n");
    printf("WorkLoad 3 on Core 0 is completed.\n\n");

    //4. w1, c0; w2, c1
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 1 on Core 0 and WorkLoad 2 on Core 1 started\n");
    printf("WorkLoad 1 on Core 0 and WorkLoad 2 on Core 1 started.\n");

    register_workload(0, workload1_init, workload1_body, workload1_exit);
    register_workload(1, workload2_init, workload2_body, workload2_exit);
    run_and_measure();
    unregister_workload_all();

    fprintf(fp, "WorkLoad 1 on Core 0 and WorkLoad 2 on Core 1 are completed\n\n");
    printf("WorkLoad 1 on Core 0 and WorkLoad 2 on Core 1 are completed.\n\n");

    //5. w1, c0; w3, c1
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 1 on Core 0 and WorkLoad 3 on Core 1 started\n");
    printf("WorkLoad 1 on Core 0 and WorkLoad 3 on Core 1 started.\n");

    register_workload(0, workload1_init, workload1_body, workload1_exit);
    register_workload(1, workload3_init, workload3_body, workload3_exit);
    run_and_measure();
    unregister_workload_all();

    fprintf(fp, "WorkLoad 1 on Core 0 + WorkLoad 3 on Core 1 are completed\n\n");
    printf("WorkLoad 1 on Core 0 + WorkLoad 3 on Core 1 are completed.\n\n");
    
    //6. w2, c0; w3, c1
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 1 on Core 0 and WorkLoad 3 on Core 1 started\n");
    printf("WorkLoad 1 on Core 0 and WorkLoad 3 on Core 1 started.\n");
    register_workload(0, workload2_init, workload2_body, workload2_exit);
    register_workload(1, workload3_init, workload3_body, workload3_exit);
    run_and_measure();
    unregister_workload_all();

    fprintf(fp, "WorkLoad 2 on Core 0 + WorkLoad 3 on Core 1 are completed\n\n");
    printf("WorkLoad 2 on Core 0 + WorkLoad 3 on Core 1 are completed.\n\n");
   
    //7. w1, c0; w1, c1
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 1 on Core 0 and WorkLoad 1 on Core 1 started\n");
    printf("WorkLoad 1 on Core 0 and WorkLoad 1 on Core 1 started.\n");
    register_workload(0, workload1_init, workload1_body, workload1_exit);
    register_workload(1, workload1_init, workload1_body, workload1_exit);
    run_and_measure();
    unregister_workload_all();

    fprintf(fp, "WorkLoad 1 on Core 0 + WorkLoad 1 on Core 1 are completed\n\n");
    printf("WorkLoad 1 on Core 0 + WorkLoad 1 on Core 1 are completed.\n\n");
   
    //8. w2, c0; w2, c1
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 2 on Core 0 and WorkLoad 2 on Core 1 started\n");
    printf("WorkLoad 2 on Core 0 and WorkLoad 2 on Core 1 started.\n");
    
    register_workload(0, workload2_init, workload2_body, workload2_exit);
    register_workload(1, workload2_init, workload2_body, workload2_exit);
    run_and_measure();
    unregister_workload_all();

    fprintf(fp, "WorkLoad 2 on Core 0 + WorkLoad 2 on Core 1 are completed\n\n");
    printf("WorkLoad 2 on Core 0 + WorkLoad 2 on Core 1 are completed.\n\n");
  
    //9. w3, c0; w3, c1
    mytime = time(NULL);
    time_str = ctime(&mytime);
    fprintf(fp, "Run on %s\r\n", time_str);
    fprintf(fp, "WorkLoad 3 on Core 0 and WorkLoad 3 on Core 1 started\n");
    printf("WorkLoad 3 on Core 0 and WorkLoad 3 on Core 1 started.\n");

    register_workload(0, workload3_init, workload3_body, workload3_exit);
    register_workload(1, workload3_init, workload3_body, workload3_exit);
    run_and_measure();
    unregister_workload_all();

    fprintf(fp, "WorkLoad 3 on Core 0 + WorkLoad 3 on Core 1 are completed\n\n");
    printf("WorkLoad 3 on Core 0 + WorkLoad 3 on Core 1 are completed.\n\n");
  
    /*******************************************************************
    * Finish
    *******************************************************************/
    set_ondemand_governor();
    fclose(fp);

    return 0;
}
