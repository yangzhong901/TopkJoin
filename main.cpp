# include<stdio.h>
#include <sys/timeb.h>
#include "dpjoin.h"

int main(int argc, char* argv[])
{
	if (argc < 4) {
		cerr << "Usage: ppjoin [sim_func] [threshold] [binary_input_file]" << endl;
		system("pause");
		return 1;
	}

	dpjoin dpjoinInstance;
	if (argc == 5) {
		if (dpjoinInstance.ReadParameters(argv[1], argv[2], argv[4])) {
			cerr << "Similarity Function Missing!" << endl;
			system("pause");
			return 1;
		}
	}
	else if (argc == 6) {
		if (dpjoinInstance.ReadParameters(argv[1], argv[2],argv[4], argv[5])) {
			cerr << "Similarity Function Missing!" << endl;
			system("pause");
			return 1;
		}
	}

	cerr << "Document: " << argv[3] << endl;
	timeb timeStart, timeEnd;	
	ftime(&timeStart);
	dpjoinInstance.ReadData(argv[3]);
	dpjoinInstance.CalcPrefix();
	ftime(&timeEnd);
	
	double initialTime = double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3;
	cerr << "ReadData Time: " << initialTime << endl;

	ftime(&timeStart);
	cerr << "=== BEGIN KJOIN (TIMER STARTED) ===" << endl;
	dpjoinInstance.PerformTopkJoin();
	cerr << "=== END JOIN (TIMER STOPPED) ===" << endl;
	ftime(&timeEnd);
	cerr << "Total Running Time: " << setiosflags(ios::fixed) << setprecision(3) <<  double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;
	cerr << "IndexEntry:" << dpjoinInstance.GetListSize() << endl;
	cerr << endl;

	dpjoinInstance.Clear();
	system("pause");

	ftime(&timeStart);
	cerr << "=== BEGIN 2KJOIN (TIMER STARTED) ===" << endl;
	dpjoinInstance.PerformTopkDualJoin();
	cerr << "=== END JOIN (TIMER STOPPED) ===" << endl;
	ftime(&timeEnd);
	cerr << "Total Running Time: " << setiosflags(ios::fixed) << setprecision(3) <<  double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;
	cerr << "IndexEntry:" << dpjoinInstance.GetListSize() << endl;	
	cerr << endl;


	dpjoinInstance.Clear();
	system("pause");

	ftime(&timeStart);
	cerr << "=== BEGIN DKJOIN (TIMER STARTED) ===" << endl;
	dpjoinInstance.PerformTopkDeltaJoin();
	cerr << "=== END JOIN (TIMER STOPPED) ===" << endl;
	ftime(&timeEnd);
	cerr << "Total Running Time: " << setiosflags(ios::fixed) << setprecision(3) <<  double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;
	cerr << "IndexEntry:" << dpjoinInstance.GetListSize() << endl;
	cerr << endl;

	system("pause");

	return 0;
}