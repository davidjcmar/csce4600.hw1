#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#define KB 1024

using namespace std;

int main (void)
{
	int times[11][2];
	char* buffer;
	FILE* infile;
	FILE* outfile;
	//string gen_file = "dd if=/dev/urandom of=test bs=KB count=";
	string gen_file = "dd if=/dev/urandom bs=1024 count=";

	/* genertae file */
	for (int i=1; i<KB+1; i=i*2)
	{
		stringstream convert_itoa;
		string gen_f;
		convert_itoa<<i;
		gen_f = gen_file + convert_itoa.str() + " of=" + convert_itoa.str();
		//cout<<gen_f<<"\n";
		system(gen_f.c_str());
		gen_f = convert_itoa.str();
		/* set dynamic values*/
		infile = fopen(gen_f.c_str(), "r");
		gen_f += "_out";
		outfile = fopen(gen_f.c_str(), "w");
		buffer = (char*)malloc(sizeof(char) * (i*KB)+1);
		/* system call */

		/* library call */
		fread(buffer, 1, (i*KB)+1, infile);
		fwrite(buffer, 1, (i*KB)+1, outfile);

		/* testing */
		cout<<"buffer size: "<<sizeof(buffer)<<" -- "<<strlen(buffer)<<"\n";
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