#include "videoTracker.h"

void parse(char * fileName,std::vector<float>* ValuesVect,struct matrixDesc* MDSC){
 	wxString Stringa;
	wxFFile file(fileName,"r");
	file.ReadAll(&Stringa);

	int nMatrix = Stringa.Freq(']');

	//MDSC = new struct matrixDesc[nMatrix];
	//std::vector<float> ValuesVect;
	
	for (int i=0;i<nMatrix;i++){
		wxString Matrix = Stringa.AfterFirst('[');
		Matrix = Matrix.BeforeFirst(']');
		int s = Matrix.size();
		Stringa.Remove(0,s+2);
		
		Matrix += ';';
		int nRows = Matrix.Freq(';');
		MDSC[i].nRows=nRows;
		for (int j=0;j<nRows;j++){
			wxString Row = Matrix.BeforeFirst(';');
			int z;
			z = Row.size();
			Matrix.Remove(0,z+1);
			
			Row +=(',');
			int nCols = Row.Freq(',');
			MDSC[i].nCols=nCols;
			for (int h=0;h<nCols;h++){
				wxString Col = Row.BeforeFirst(',');
				float val = strtod(Col,NULL);
				ValuesVect->push_back(val);

				int p;
				p = Col.size();
				Row.Remove(0,p+1);

				}
			}
		}	

}