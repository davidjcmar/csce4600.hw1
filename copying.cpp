#include <iostream>
#include <unistd.h>
#include <fcntl.h>
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
	int byte_num;
	int fd_in, fd_out;
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
		string make_file;
		convert_itoa<<"/tmp/"<<i;
		gen_f = gen_file + convert_itoa.str() + " of=" + convert_itoa.str() + " >/dev/null 2>&1";

		system(gen_f.c_str()); // system call to dd to quietly generate i * KB (1024) bits quietly
		gen_f = convert_itoa.str();
		
		/* system call */
		buffer = (char*)malloc(sizeof(char) * (i*KB));
		fd_in = open(gen_f.c_str(), O_RDONLY);
		gen_f += "_out";
		make_file = "touch " + gen_f;
		system(make_file.c_str());
		fd_out = open(gen_f.c_str(), O_WRONLY);
		
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time) == 0)
		{
			byte_num = read(fd_in, buffer, (i*KB));
			byte_num=write(fd_out, buffer, byte_num);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop_time);
			elapse_time = stop_time.tv_sec - start_time.tv_sec;
			elapse_time += ((stop_time.tv_nsec/1E9) - (start_time.tv_nsec/1E9));
			cout<<"sys: "<<byte_num<<"->"<<elapse_time<<"\n"; // output elapsed time
		}
		else
		{
			cout<<"ERROR: Clock failed.\n";
			return 1;
		}
		free (buffer);
		close (fd_in);
		close (fd_out);
		
		/* init files and buffer*/
		infile = fopen(gen_f.c_str(), "r");
		//gen_f += "_out";
		outfile = fopen(gen_f.c_str(), "w");
		buffer = (char*)malloc(sizeof(char) * (i*KB));
	



		/* reinit files and buffer */
		gen_f = convert_itoa.str();
		gen_f += "_fout";
		outfile = fopen(gen_f.c_str(), "w"); //open output file
		//memset(buffer,'\n', (i*KB)); // init buffer
		/* library call */
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time) == 0)
		{	
			fread(buffer, 1, (i*KB), infile);
			fwrite(buffer, 1, (i*KB), outfile);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop_time);
			elapse_time = stop_time.tv_sec - start_time.tv_sec;
			elapse_time += ((stop_time.tv_nsec/1E9) - (start_time.tv_nsec/1E9));
			cout<<"lib: "<<i*KB<<"->"<<elapse_time<<"\n"; // output elapsed time
		}
		else
		{
			cout<<"ERROR: Clock failed.\n";
			return 1;
		}
		/* cleanup */
		free(buffer);
		fclose(infile);
		fclose(outfile);
	}
	return 0;
}