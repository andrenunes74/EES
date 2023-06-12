
/*
 *  Análise e Teste de Software
 *  João Saraiva 
 *  2016-2017
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "rapl.h"
#include <regex.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <powercap/powercap.h>
#define RUNTIME


int main (int argc, char **argv) 
{ char command[500],res[500];
  int  ntimes = 1;
  int  core = 0;
  int  i=0;

#ifdef RUNTIME
  clock_t begin, end;
  double time_spent;

  struct timeval tvb,tva;
  
#endif
  
  FILE * fp;


  
  // Initialize powercap and set power limit
  powercap_control_type control;
  powercap_zone zone;
  powercap_constraint constraint;
  
  // Open control type file to enable control
  powercap_control_type_file_open(&control, POWERCAP_CONTROL_TYPE_FILE_ENABLED, "intel-rapl", O_RDWR);

  // Enable the control
  powercap_control_type_set_enabled(&control, 1);

  // Open power zone file to enable power zone
  powercap_zone_file_open(&zone, POWERCAP_ZONE_FILE_ENABLED, "intel-rapl", NULL, 0, O_RDWR);

  // Enable the zone
  powercap_zone_set_enabled(&zone, 1);

  // Open constraint file to set power limit
  powercap_constraint_file_open(&constraint, POWERCAP_CONSTRAINT_FILE_POWER_LIMIT_UW, "intel-rapl", NULL, 0, 0, O_RDWR);

  // Set power limit to a specific value, e.g. 10000 uW
  int powercap = 60000000;
  powercap_constraint_set_power_limit_uw(&constraint, powercap);


  //printf("Program to be executed: %d",argc);
  //strcpy( command, "./" );
  strcat(command,argv[1]);
  printf("Program to be executed: %s\n",argv[1]);

  strcpy(command, "./" );
  strcat(command,argv[1]);

  ntimes = atoi (argv[2]);

  strcpy(res,command);
  strcat(res,".J");
  printf("Command: %s  %d-times res: %s\n",command,ntimes,res);
  

  printf("\n\n RUNNING THE PARAMETRIZED PROGRAM:  %s\n\n\n",command);
  fflush(stdout);
  
  fp = fopen(res,"w");
  rapl_init(core);

   fprintf(fp, "Program, Package , Core(s) , GPU , DRAM? , Time (sec), Peak Memory Usage (KB), PowerCap (uW)\n");

  for (i = 0 ; i < ntimes ; i++)
    {
        fprintf(fp,"%s , ",argv[1]);
        int attempt = 0;
        int temp = 100;
        char buffer[1024];
        //wait 4 seconds or until temperature is below 75°C
        while(temp>75 && attempt<=4){
          //use popen and fgets to store the temperature of executing "sensors" cmd
          FILE* sensors_output = popen("sensors | grep 'Package'", "r");
          if(sensors_output == NULL){
              printf("Error!\n");
              return 1;
          }
          fgets(buffer, sizeof(buffer), sensors_output);
          pclose(sensors_output);
          sscanf(buffer, "Package id 0:  + %d °C", &temp);
          //first case scenario
          if(temp<=65)break;

          printf("Waiting to cooldown... Last measured temperature: %d°C\n",temp);
          sleep(1);
          attempt++;
        }
        printf("Temperature: %d °C\n",temp);
        rapl_before(fp,core);
      
#ifdef RUNTIME
        begin = clock();
	gettimeofday(&tvb, 0);
#endif

        //adicionei
        // Start measuring memory usage
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        long peak_memory_usage = usage.ru_maxrss;

	
        system(command);

#ifdef RUNTIME
	end = clock();
	gettimeofday(&tva, 0);
	//	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	time_spent = (tva.tv_sec-tvb.tv_sec)*1000000 + tva.tv_usec-tvb.tv_usec;
#endif

	rapl_after(fp,core);

#ifdef RUNTIME    
        //adicionei
        fprintf(fp, " %G, %ld, %d \n", time_spent, peak_memory_usage, powercap);


#endif  
    }
    
  printf("\n\n END of PARAMETRIZED PROGRAM: \n");

  fclose(fp);
  fflush(stdout);

  return 0;
}



