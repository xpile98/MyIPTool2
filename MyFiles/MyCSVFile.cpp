#include "stdafx.h"
#include "MyCSVFile.h"

void CSV::CSVtest()
{
	fp = fopen("test.csv", "w+");
	fprintf(fp, "�̸�,�ּ�,����,\n");
	fprintf(fp, "�̸�,�ּ�,����,\n");
	fclose(fp);
}

void CSV::MakeFile(char *fn)
{
	fp = fopen(fn, "w+");
	fclose(fp);
}

void CSV::SetKey(char * fn)
{
	fprintf(fp, fn);
}

void CSV::SetNode(char * fn)
{
	fprintf(fp, fn);
}
