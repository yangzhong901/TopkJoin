#include "dpjoin.h"


int dpjoin::Getk()
{
	return k;
}

int dpjoin::ReadParameters(char* sim_func, char* threshold, char* k, char* delta)
{
	if (sim_func[0] == 'j' || sim_func[0] == 'J') jaccard = 1;
	else if (sim_func[0] == 'c' || sim_func[0] == 'C') cosine = 1;
	else if (sim_func[0] == 'o' || sim_func[0] == 'O') overlap = 1;
	else return 1;
	dpjoin::k = atoi(k);
	dpjoin::fixedelta = atoi(delta);
	dpjoin::threshold = atof(threshold);
	cerr << "Similarity Function: ";
	if (jaccard == 1) cerr << "Jaccard" << endl;
	else if (cosine == 1) cerr << "Cosine" << endl;
	else if (overlap == 1) cerr << "Overlap" << endl;
	cerr << "Threshold: " << dpjoin::threshold << endl;
	return 0;
}

int dpjoin::ReadParameters(char* sim_func, char* threshold, char* k)
{
	if (sim_func[0] == 'j' || sim_func[0] == 'J') jaccard = 1;
	else if (sim_func[0] == 'c' || sim_func[0] == 'C') cosine = 1;
	else if (sim_func[0] == 'o' || sim_func[0] == 'O') overlap = 1;
	else return 1;
	dpjoin::threshold = atof(threshold);
	dpjoin::k = atoi(k);
	cerr << "Similarity Function: ";
	if (jaccard == 1) cerr << "Jaccard" << endl;
	else if (cosine == 1) cerr << "Cosine" << endl;
	else if (overlap == 1) cerr << "Overlap" << endl;
	cerr << "Threshold: " << dpjoin::threshold << endl;
	return 0;
}

void dpjoin::ReadData(char* filename)
{
	int i, rid, rlen;
	int totalLen = 0;
	int inputToken;
	record inputRecord;
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, filename, "rb");

	/* read binary data */
	while (fread(&rid, sizeof(int), 1, fp) == 1) {
		fread(&rlen, sizeof(int), 1, fp);
		totalLen += rlen;
		inputRecord.rid = rid, inputRecord.len = rlen, inputRecord.tokenArray.clear();
		for (i = 0; i < rlen; i++) {
			fread(&inputToken, sizeof(int), 1, fp);
			if (inputToken > tokenNum) tokenNum = inputToken;
			inputRecord.tokenArray.push_back(inputToken);
		}
		joinRecord.push_back(inputRecord);
		++recordNum;
	}
	fclose(fp);
	++tokenNum;

	/* output statistics */
	cerr << "# Records: " << recordNum << endl;
	cerr << "# Average Record Size: " << double(totalLen) / recordNum << endl;
	cerr << "# Maximum Record Size: " << joinRecord[recordNum - 1].len << endl;
}

void dpjoin::WriteResults()
{
	//int i;
	vector<result>::iterator it;
	for (it = joinResult.begin(); it != joinResult.end(); ++it)
		cout << it->xid << " " << it->yid << " " << setiosflags(ios::fixed) << setprecision(3) << it->sim << endl;
}

void dpjoin::WriteData(vector<record> joinRecord)
{
	char *filename= "./test.txt.bin";
	FILE *fp;
	int rid, rlen, token;
	errno_t err;
	err = fopen_s(&fp, filename, "wb");
	if (err) {
		cout << "open file error£¡" << endl;
		exit(0);
	}
	for (size_t i = 0; i < joinRecord.size(); i++)
	{
		record rec = joinRecord[i];
		rid = rec.rid;
		rlen = (int)rec.tokenArray.size();
		if (fwrite(&rid, sizeof(int), 1, fp) != 1) {
			cout << "write rid error:" + to_string(i) << endl;
		}
		if (fwrite(&rlen, sizeof(int), 1, fp) != 1) {
			cout << "write rlen error:" + to_string(i) << endl;
		}
		for (size_t j = 0; j < rec.tokenArray.size(); j++)
		{
			token = rec.tokenArray[j];
			if (fwrite(&token, sizeof(int), 1, fp) != 1) {
				cout << "write token error:" + to_string(j) << endl;
			}
		}
	}
	
	fclose(fp);

}



