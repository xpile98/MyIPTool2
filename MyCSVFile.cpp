#include "stdafx.h"
#include "MyCSVFile.h"

void CSV::CSVtest()
{
	fp = fopen("test.csv", "w+");
	fprintf(fp, "이름,주소,나이,\n");
	fprintf(fp, "이름,주소,나이,\n");
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
