#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>
#include <iomanip>
#define KB 1024

using namespace std;

int main (void)
{
	int times[11][2];
	char* buffer;
	FILE* infile;
	FILE* outfile;
	struct timespec start_time, stop_time;
	double elapse_time;
	string gen_file = "dd if=/dev/urandom bs=1024 count=";

	/* genertae file */
	for (int i=1; i<KB+1; i=i*2)
	{
		stringstream convert_itoa;
		string gen_f;
		convert_itoa<<i;
		gen_f = gen_file + convert_itoa.str() + " of=" + convert_itoa.str() + " >/dev/null 2>&1";

		system(gen_f.c_str()); // system call to dd to quietly generate i * KB (1024) bits quietly
		gen_f = convert_itoa.str();

		/* set dynamic values*/
		infile = fopen(gen_f.c_str(), "r");
		gen_f += "_out";
		outfile = fopen(gen_f.c_str(), "w");
		buffer = (char*)malloc(sizeof(char) * (i*KB)+1);
		/* system call */

		/* library call */
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time) == 0)
		{	
			fread(buffer, 1, (i*KB)+1, infile);
			fwrite(buffer, 1, (i*KB)+1, outfile);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop_time);
			elapse_time = stop_time.tv_sec - start_time.tv_sec;
			elapse_time += ((stop_time.tv_nsec/1E9) - (start_time.tv_nsec/1E9));
			cout<<i*KB<<"->"<<elapse_time<<"\n"; // output elapsed time
		}
		else
		{
			cout<<"ERROR: Clock failed.\n";
			return 1;
		}
		/* testing */
		//cout<<"buffer size: "<<sizeof(buffer)<<" -- "<<strlen(buffer)<<"\n";
		//cout<<buffer<<"\n";
		/* free allocated buffer */

		/* cleanup */
		fclose(infile);
		fclose(outfile);
		free(buffer);
		//system("rm test");
	}
	return 0;
}