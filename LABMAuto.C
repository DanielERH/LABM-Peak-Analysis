//I am using root 6.14/04 to run this script, the instruction from terminal is "root -l -b -q LABMAuto.C", the "-l" is to skip root intro, "-b" is to not open the graphs while the program is running and "-q" is to close root once its finished. Also, it needs the configuration file "labmpeaks.conf" in the same directory, it needs a folder named "DATA" and another one named "PDF" in the same directory and it needs the data files (for example "labmpeaks-run00000100_0000.dat") in the Data folder, so it should look like this

// HEADFOLDER (folder)
//  - LABMAuto.C
//  - labmpeaks.conf
//  - DATA (folder)
//     - labmpeaks-run00000100_0000.dat
//     - labmpeaks-run00000100_0001.dat
//     - ...
//  - PDF (folder)

//*NOTE* I noticed that in some "labmpeaks.conf" there were some peaks repeated, be sure to NOT repeat the peaks when creating the labmpeaks.conf, IF there are some peaks repeated there will be errors with this code and final results will be WRONG... In case there are peaks repeated the program will close itself and there will be a message saying what peaks to edit in the file, make those peaks either correct (I noticed some of them were repeated because the steps were not increased) or change them to another number that is not 0

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <ctime>

using namespace std;

int op=0, ou=0, od=0, nu=0, nd=0, oustatus=0, odstatus=0, nustatus=0, ndstatus=0, dummy1=0, dummy2=0, peaksnumber=0, cycle=0, cyou=0, cyod=0, cynu=0, cynd=0, filenum=0, motor1=0, motor2=0, motor3=0, motor4=0, motor9=0, motor10=0, motor11=0, motor12=0, rate=0, numlines=0, fail=0, datacounter=0, safe=0, nbin1=0, nbin2=0, ousec=0, odsec=0, nusec=0, ndsec=0, OUsec[10]={0}, ODsec[10]={0}, NUsec[10]={0}, NDsec[10]={0}, pmtCounter=0, datatime=0, LcyOU[10]={0}, FcyOU[10]={0}, LcyOD[10]={0}, FcyOD[10]={0}, LcyNU[10]={0}, FcyNU[10]={0}, LcyND[10]={0}, FcyND[10]={0};

Float_t peaksOU[1000][12]={0}, peaksOD[1000][12]={0}, peaksNU[1000][12]={0}, peaksND[1000][12]={0};
Float_t peaksOUMatrix[25000][1000][12]={0}, peaksODMatrix[25000][1000][12]={0}, peaksNUMatrix[25000][1000][12]={0}, peaksNDMatrix[25000][1000][12]={0};
Float_t peaksOUMax[10][25000][12]={0}, peaksODMax[10][25000][12]={0}, peaksNUMax[10][25000][12]={0}, peaksNDMax[10][25000][12]={0};

Float_t motor1r[10]={0}, motor2r[10]={0}, motor3r[10]={0}, motor4r[10]={0}, motor9r[10]={0}, motor10r[10]={0}, motor11r[10]={0}, motor12r[10]={0}, r1[10][25000][12]={0}, r2[10][25000][12]={0}, r3[10][25000][12]={0};

Float_t OUPMTMax[10][25000][3][12]={0}, ODPMTMax[10][25000][3][12]={0}, NUPMTMax[10][25000][3][12]={0}, NDPMTMax[10][25000][3][12]={0};

Float_t Lcurrent=0, Hcurrent=0, PMTMax1=0, PMTMax2=0, PMTMin1=0, PMTMin2=0;

Float_t m0[10][25000][12]={0}, Motxave[10][25000][12]={0}, Motyave[10][25000][12]={0}, DMotxave[10][25000][12]={0}, DMotyave[10][25000][12]={0}, AMotxMoty[10][25000][12]={0}, m0max[12]={0}, m0min[12]={0}, Motxavemax[12]={0}, Motxavemin[12]={0}, Motyavemax[12]={0}, Motyavemin[12]={0}, DMotxavemax[12]={0}, DMotxavemin[12]={0}, DMotyavemax[12]={0}, DMotyavemin[12]={0}, AMotxMotymax[12]={0}, AMotxMotymin[12]={0};

char dummy3[50], n[200];

stringstream sssection;
string nombrepdf;
string section;
string peaksnumberfile;

int LABMAuto(){
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This part of the code is to read the configuration file and fill the peaks matrix with motor positions

	fstream labmpeaks ("labmpeaks.conf"); //Open the file **** REMEMBER TO CHANGE THIS DIRECTION

	do{
		labmpeaks.ignore(250, '\n');
		labmpeaks >> dummy3;
		if (strcmp(dummy3,"LABMLINE")==0)	break; //Ignore until reach to the line that starts with the word "LABMLINE"
	}while (true);
	labmpeaks >> dummy1;
	labmpeaks >> oustatus; //save the Oho Up status
	labmpeaks >> dummy1;
	labmpeaks >> dummy1;
	labmpeaks >> dummy3;
	labmpeaks >> dummy1;
	labmpeaks >> odstatus; //save the Oho Down status
	labmpeaks >> dummy1;
	labmpeaks >> dummy1;
	labmpeaks >> dummy3;
	labmpeaks >> dummy1;
	labmpeaks >> nustatus; //save the Nikko Up status
	labmpeaks >> dummy1;
	labmpeaks >> dummy1;
	labmpeaks >> dummy3;
	labmpeaks >> dummy1;
	labmpeaks >> ndstatus; //save the Nikko Down status
	labmpeaks >> dummy1;
	labmpeaks >> dummy1;
	do{
		labmpeaks.ignore(250, '\n');
		labmpeaks >> dummy3;
		if (strcmp(dummy3,"LABMPEAKS")==0)	break; //Ignore until reach to the line that starts with the word "LABMPEAKS"
	}while (true);
	do{
		labmpeaks >> op; //Save optical channel number
		labmpeaks >> dummy1;
		if (op==1){ //If "op" is 1 it means that the peak is from Oho Up
			ou++;// Increase the ou peak counter
			labmpeaks >> peaksOU[ou][1]; //Save Motor 1 steps position for peak "ou"
			labmpeaks >> dummy2;
			labmpeaks >> peaksOU[ou][2]; //Save Motor 2 steps position for peak "ou"
		}
		if (op==2){ //Repeat for Oho Down
			od++;
			labmpeaks >> peaksOD[od][1]; //Motor 3
			labmpeaks >> dummy2;
			labmpeaks >> peaksOD[od][2]; //Motor 4
		}
		if (op==3){ //Repeat for Nikko Up
			nu++;
			labmpeaks >> peaksNU[nu][1]; //Motor 9
			labmpeaks >> dummy2;
			labmpeaks >> peaksNU[nu][2]; //Motor 10
		}
		if (op==4){ //Repeat for Nikko Down
			nd++;
			labmpeaks >> peaksND[nd][1]; //Motor 11
			labmpeaks >> dummy2;
			labmpeaks >> peaksND[nd][2]; //Motor 12
		}
		labmpeaks >> dummy3;
		if (strcmp(dummy3,"!")==0){ //If the first "word" or "character" is "!" it will ignore and grab the next line word
			labmpeaks.ignore(250, '\n');
			labmpeaks >> dummy3;
			if (strcmp(dummy3,"PEAKSDATADISK")==0) break;//Ignore until reach to line that starts with the word "PEAKSDATADISK"
		}
	}while (true); //Repeat until "break" 
	do{
		labmpeaks.ignore(250, '\n');
		labmpeaks >> dummy3;
		if (strcmp(dummy3,"PEAKSRUNNUMBER")==0)	labmpeaks >> peaksnumber;//Ignore until reach to line with the word "PEAKSRUNNUMBER"
		if (strcmp(dummy3,"PEAKSTIMESTAMP")==0)	break; //Ignore until reach to line with the word "PEAKSTIMESTAMP"
	}while (true); //Repeat until "break" 
	labmpeaks.close(); //Close the file

	peaksnumberfile= to_string(peaksnumber); //Make a string out of the peaksnumber for pdf names at the end

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This part of the code is to check that there are no repeated peaks in the configuration file

for (int i=1; i<=ou; i++){ //We are comparing each peak
	for (int j=1; j<=ou; j++){ //To all the other peaks
		if (i!=j){ //Make sure to not compare a peak with itself
			if ((peaksOU[i][1]==peaksOU[j][1]) and (peaksOU[i][2]==peaksOU[j][2])){ //If the motor position is the same
				cout << "Peaks " << i << " and " << j << " are repeated in Oho Up, fix is needed" << endl;
				cout << "Motor 1 " << peaksOU[i][1] << endl;
				cout << "Motor 2 " << peaksOU[i][2] << endl;
				return(0); //Close the program
			}
		}
	}
}
for (int i=1; i<=od; i++){ //Repeat for Oho Down
	for (int j=1; j<=od; j++){
		if (i!=j){
			if ((peaksOD[i][1]==peaksOD[j][1]) and (peaksOD[i][2]==peaksOD[j][2])){
				cout << "Peaks " << i << " and " << j << " are repeated in Oho Down, fix is needed" << endl;
				cout << "Motor 3 " << peaksOD[i][1] << endl;
				cout << "Motor 4 " << peaksOD[i][2] << endl;
				return(0);
			}
		}
	}
}
for (int i=1; i<=nu; i++){ //Repeat for Nikko Up
	for (int j=1; j<=nu; j++){
		if (i!=j){
			if ((peaksNU[i][1]==peaksNU[j][1]) and (peaksNU[i][2]==peaksNU[j][2])){
				cout << "Peaks " << i << " and " << j << " are repeated in Nikko Up, fix is needed" << endl;
				cout << "Motor 9 " << peaksNU[i][1] << endl;
				cout << "Motor 10 " << peaksNU[i][2] << endl;
				return(0);
			}
		}
	}
}
for (int i=1; i<=nd; i++){ //Repeat for Nikko Down
	for (int j=1; j<=nd; j++){
		if (i!=j){
			if ((peaksND[i][1]==peaksND[j][1]) and (peaksND[i][2]==peaksND[j][2])){
				cout << "Peaks " << i << " and " << j << " are repeated in Nikko Down, fix is needed" << endl;
				cout << "Motor 11 " << peaksND[i][1] << endl;
				cout << "Motor 12 " << peaksND[i][2] << endl;
				return(0);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This part of the code is to divide in areas in case we have more than only 1 area that we are scanning in each channel

	for (int i=1; i<ou; i++){ //From all the peaks if the diff between the peak i and the next peak is bigger than 2000 or if the peak i is bigger than the next peak that means that we jumped from 1 area of scan to the next
		if ((abs(peaksOU[i][1]-peaksOU[i+1][1])>2000) or (peaksOU[i][1]>peaksOU[i+1][1])){
			if (peaksOU[i+1][1]!=0){ //Also check if the next peak position is 0 (it means that there is no next peak!)
				ousec++; //increase area counter for Oho Up
				OUsec[ousec]=i; //Save the last peak of each area
			}
		}
	}
	for (int i=1; i<od; i++){ //Repeat for Oho Down
		if ((abs(peaksOD[i][1]-peaksOD[i+1][1])>2000) or (peaksOD[i][1]>peaksOD[i+1][1])){
			if (peaksOD[i+1][1]!=0){
				odsec++;
				ODsec[odsec]=i;
			}
		}
	}
	for (int i=1; i<nu; i++){//Repeat for Nikko Up
		if ((abs(peaksNU[i][1]-peaksNU[i+1][1])>2000) or (peaksNU[i][1]>peaksNU[i+1][1])){
			if (peaksNU[i+1][1]!=0){
				nusec++;
				NUsec[nusec]=i;
			}
		}
	}
	for (int i=1; i<nd; i++){//Repeat for Nikko Down
		if ((abs(peaksND[i][1]-peaksND[i+1][1])>2000) or (peaksND[i][1]>peaksND[i+1][1])){
			if (peaksND[i+1][1]!=0){
				ndsec++;
				NDsec[ndsec]=i;
			}
		}
	}

	OUsec[ousec+1]=ou; //Make the last one the last peak of Oho up
	ODsec[odsec+1]=od; //Make the last one the last peak of Oho Down
	NUsec[nusec+1]=nu; //Make the last one the last peak of Nikko up
	NDsec[ndsec+1]=nd; //Make the last one the last peak of Nikko Down


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This part of the code is to read the data files starting from file 0 until it can't open a new data file. The code will read the first word of each line, if the word is "Number" it will do the part of the code to save the motors positions and save the first and last data entry, if the word is "PMRecord" it will do the part of the code to save the rates and if the word is "BMLDCCT:CURRENT" it will normalize the rates
	do{
		sprintf(n, "DATA/%i/labmpeaks-run%08i_%04i.dat", peaksnumber, peaksnumber, filenum); //Open the data file *** REMEMBER TO CHANGE THIS DIRECTION
		fstream data (n);
		if (data.is_open()){ //If the datais open
			do{
				data >> dummy3; //Save the first word of the line we are on
				if (strcmp(dummy3,"Number")==0){ //If the word is "Number"
					for(int i=1; i<=ou; i++){ //For all peaks un Oho Up
						if ((peaksOU[i][1]==motor1) and (peaksOU[i][2]==motor2)){//Search for the peak of the data
							peaksOUMatrix[cyou][i][11]=datatime; //Save the seconds
							if(datacounter-fail!=0){ //Check that the data is not Null
								for(int j=0; j<8; j++){ //Save the average of each pmt
								peaksOUMatrix[cyou][i][j+3]=peaksOUMatrix[cyou][i][j+3]/(datacounter-fail);
								}
							}
						}
					}
					for(int i=1; i<=od; i++){//Repeat for Oho Down
						if ((peaksOD[i][1]==motor3) and (peaksOD[i][2]==motor4)){
							peaksODMatrix[cyod][i][11]=datatime;
							if(datacounter-fail!=0){
								for(int j=0; j<8; j++){
								peaksODMatrix[cyod][i][j+3]=peaksODMatrix[cyod][i][j+3]/(datacounter-fail);
								}
							}
						}
					}
					for(int i=1; i<=nu; i++){//Repeat for Nikko Up
						if ((peaksNU[i][1]==motor9) and (peaksNU[i][2]==motor10)){
							peaksNUMatrix[cynu][i][11]=datatime;
							if(datacounter-fail!=0){
								for(int j=0; j<8; j++){
								peaksNUMatrix[cynu][i][j+3]=peaksNUMatrix[cynu][i][j+3]/(datacounter-fail);
								}
							}
						}
					}
					for(int i=1; i<=nd; i++){//Repeat for Nikko Down
						if ((peaksND[i][1]==motor11) and (peaksND[i][2]==motor12)){
							peaksNDMatrix[cynd][i][11]=datatime;
							if(datacounter-fail!=0){
								for(int j=0; j<8; j++){
								peaksNDMatrix[cynd][i][j+3]=peaksNDMatrix[cynd][i][j+3]/(datacounter-fail);
								}
							}
						}
					}
					if(ou!=0){ //If the peak counter is not zero
						if (cycle%ou==0){ //And the residue of the cycle counter and the peak counter is zero
							cyou++; //Increase the peak cycle counter
						}
					}
					if(od!=0){
						if (cycle%od==0){
							cyod++;
						}
					}
					if(nu!=0){
						if (cycle%nu==0){
							cynu++;
						}
					}
					if(nd!=0){
						if (cycle%nd==0){
							cynd++;
						}
					}
					cycle++; //Increase the cycle counter (this means that we saved all the data of the peak)
					data >> dummy3; //Ignore
					data >> dummy3; //Ignore
					data >> dummy3; //Ignore
					data >> numlines; //Save the number of lines that are working (# of Optical Channels)
					for(int i=0; i<numlines; i++){
						data >> dummy1;//Ignore
						data >> dummy1;//Ignore
						data >> dummy1;//Ignore
						if (dummy1==2){ //If the data is 2 it means that line was for Oho Up
							data >> motor1; //Save Data file motor1 position
							data >> motor2; //Save Data file motor2 position
						}
						if (dummy1==4){
							data >> motor3;
							data >> motor4;
						}

						if (dummy1==10){
							data >> motor9;
							data >> motor10;
						}

						if (dummy1==12){
							data >> motor11;
							data >> motor12;
						}
					}
					fail=0; //Reset fail counter
					datacounter=0; //Reset datacounter
				}
				if (strcmp(dummy3,"PMRecord")==0){ //If the word is "PMRecord"
					datacounter++; //It means that we are getting data, so increase the datacounter
					datatime++; //Increase the datatime counter
					data.ignore(250, '\n');
					if (ou==0){ //If there are no peaks then we need to ignore the 8 rates
						for(int i=0; i<9; i++)	data.ignore(250, '\n');
					}else{ //If there are peaks 
						for(int i=1; i<=ou; i++){//Scan all the peaks to find the peak of the data by the motors
							if ((peaksOU[i][1]==motor1) and (peaksOU[i][2]==motor2)){
								safe++; //If we find one peak we increase the safe counter
								for(int j=0; j<8; j++){ //save the rate
									data >> rate;
									peaksOU[i][j+3]=rate;
								}
							}

						}
						if(safe==0){ //If didn't found a peak that match the motors position
							for(int i=0; i<9; i++)	data.ignore(250, '\n'); //we ignore the rates
						}else{
							safe=0; //Restart the counter for the next one
						}
					}
					if (od==0){ //Repeat for Oho Down
						for(int i=0; i<9; i++)	data.ignore(250, '\n');
					}else{
						for(int i=1; i<=od; i++){
							if ((peaksOD[i][1]==motor3) and (peaksOD[i][2]==motor4)){
								safe++;
								for(int j=0; j<8; j++){
									data >> rate;
									peaksOD[i][j+3]=rate;
								}
							}

						}
						if(safe==0){
							for(int i=0; i<9; i++)	data.ignore(250, '\n');
						}else{
							safe=0;
						}
					}
					if (nd==0){ //Repeat for Nikko Down
						for(int i=0; i<9; i++)	data.ignore(250, '\n');
					}else{
						for(int i=1; i<=nd; i++){
							if ((peaksND[i][1]==motor11) and (peaksND[i][2]==motor12)){
								safe++;
								for(int j=0; j<8; j++){
									data >> rate;
									peaksND[i][j+3]=rate;
								}
							}
						}
						if(safe==0){
							for(int i=0; i<9; i++)	data.ignore(250, '\n');
						}else{
							safe=0;
						}
					}
					if (nu==0){ //Repeat for Nikko Up
						for(int i=0; i<9; i++)	data.ignore(250, '\n');
					}else{
						for(int i=1; i<=nu; i++){
							if ((peaksNU[i][1]==motor9) and (peaksNU[i][2]==motor10)){
								safe++;
								for(int j=0; j<8; j++){
									data >> rate;
									peaksNU[i][j+3]=rate;
								}
							}
						}
						if(safe==0){
							for(int i=0; i<9; i++)	data.ignore(250, '\n');
						}else{
							safe=0;
						}
					}
				}

				if (strcmp(dummy3,"BMLDCCT:CURRENT")==0){ //If the word is "BMLDCCT:CURRENT"
					data >> Lcurrent; //Save LER current
					data >> dummy3; //Ignore
					data >> dummy3; //Ignore
					data >> Hcurrent; //Save HER current
					data.ignore(250, '\n');
					do{
						data.ignore(250, '\n');
						data >> dummy3;
						if (strcmp(dummy3,"COLKEKB:LER:Status:MSG")==0)	break;//Ignore until that "word"
					}while (true);
					data >> dummy3; //Save the next word
					if ((strcmp(dummy3,"Physics")!=0) or (Lcurrent<20) or (Hcurrent<20)){ //If not physics run
						fail++; //Increase the fail counter
						Lcurrent=1;
						Hcurrent=1;	
					}
					for(int i=1; i<=ou; i++){ //We need to find the peak that matches the motors
						if ((peaksOU[i][1]==motor1) and (peaksOU[i][2]==motor2)){
							if((Lcurrent==1) or (Hcurrent==1)){ //If its not physics run
								for(int j=0; j<8; j++){
									peaksOU[i][j+3]=0; //Make the rate zero
								}
							}
							for(int j=0; j<8; j++){ //Sum the normalized rate for that cycle and that peak
								peaksOUMatrix[cyou][i][j+3]+=peaksOU[i][j+3]/Hcurrent;
							}
						}
					}																
					for(int i=1; i<=od; i++){ //Repeat for Oho Down
						if ((peaksOD[i][1]==motor3) and (peaksOD[i][2]==motor4)){
							if((Lcurrent==1) or (Hcurrent==1)){
								for(int j=0; j<8; j++){
									peaksOD[i][j+3]=0;
								}
							}
							for(int j=0; j<8; j++){
								peaksODMatrix[cyod][i][j+3]+=peaksOD[i][j+3]/Hcurrent;
							}
						}
					}
					for(int i=1; i<=nu; i++){ //Repeat for Nikko Up
						if ((peaksNU[i][1]==motor9) and (peaksNU[i][2]==motor10)){
							if((Lcurrent==1) or (Hcurrent==1)){
								for(int j=0; j<8; j++){
									peaksNU[i][j+3]=0;
								}
							}
							for(int j=0; j<8; j++){
								peaksNUMatrix[cynu][i][j+3]+=peaksNU[i][j+3]/Lcurrent;
							}
						}
					}
					for(int i=1; i<=nd; i++){ //Repeat for Nikko Down
						if ((peaksND[i][1]==motor11) and (peaksND[i][2]==motor12)){
							if((Lcurrent==1) or (Hcurrent==1)){
								for(int j=0; j<8; j++){
									peaksND[i][j+3]=0;
								}
							}
							for(int j=0; j<8; j++){
								peaksNDMatrix[cynd][i][j+3]+=peaksND[i][j+3]/Lcurrent;
							}
						}
					}
				}
				data.ignore(250, '\n'); //Ignore the rest of the line
			if (!data.good()) break; //This is to stop the loop at the end of each file
			}while (true); 
			data.close(); //Close file
			filenum++; //Increase file number
		}else{
			break; //Exit the loop if there is no more data files
		}
	}while (true);

	for (int j=1; j<=cyou; j++){ //Save the motor position for each peak and peak cycle
		for(int i=1; i<=ou; i++){
			peaksOUMatrix[j][i][1]=peaksOU[i][1];
			peaksOUMatrix[j][i][2]=peaksOU[i][2];
		}
	}
	for (int j=1; j<=cyod; j++){
		for(int i=1; i<=od; i++){
			peaksODMatrix[j][i][1]=peaksOD[i][1];
			peaksODMatrix[j][i][2]=peaksOD[i][2];
		}
	}
	for (int j=1; j<=cynu; j++){
		for(int i=1; i<=nu; i++){
			peaksNUMatrix[j][i][1]=peaksNU[i][1];
			peaksNUMatrix[j][i][2]=peaksNU[i][2];
		}
	}
	for (int j=1; j<=cynd; j++){
		for(int i=1; i<=nd; i++){
			peaksNDMatrix[j][i][1]=peaksND[i][1];
			peaksNDMatrix[j][i][2]=peaksND[i][2];
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This part of the code is to save the peak with the max rate for each channel

	for (int p=0; p<=ousec; p++){ //For each area
		for(int l=1; l<=cyou; l++){ //For each cycle
			if((peaksOUMatrix[l][OUsec[p+1]][3]!=0)and(peaksOUMatrix[l][OUsec[p+1]][6]!=0)and(peaksOUMatrix[l][OUsec[p+1]][9]!=0)){
				for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){ //And each peak in that area
					for(int j=0; j<8; j++){
						if (peaksOUMatrix[l][i][j+3]>peaksOUMax[p][l][j+3]){//If the rate of that peak is bigger
							pmtCounter++; //Increase the counter
						}
					}
					if(pmtCounter>4){ //If 4 pmts rates are bigger
						for(int k=1; k<12; k++){
							peaksOUMax[p][l][k]=peaksOUMatrix[l][i][k]; //That peak is the new max
						}
					}
					pmtCounter=0; //Reset the counter for the next peak
				}
			}
		}
	}
	for (int p=0; p<=odsec; p++){ //Repeat for Oho Down
		for(int l=1; l<=cyod; l++){
			if((peaksODMatrix[l][ODsec[p+1]][3]!=0)and(peaksODMatrix[l][ODsec[p+1]][6]!=0)and(peaksODMatrix[l][ODsec[p+1]][9]!=0)){
				for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){
					for(int j=0; j<8; j++){
						if (peaksODMatrix[l][i][j+3]>peaksODMax[p][l][j+3]){
							pmtCounter++;
						}
					}
					if(pmtCounter>4){
						for(int k=1; k<12; k++){
							peaksODMax[p][l][k]=peaksODMatrix[l][i][k];
						}
					}
					pmtCounter=0;
				}
			}
		}
	}
	for (int p=0; p<=nusec; p++){ //Repeat for Nikko Up
		for(int l=1; l<=cynu; l++){
			if((peaksNUMatrix[l][NUsec[p+1]][3]!=0)and(peaksNUMatrix[l][NUsec[p+1]][6]!=0)and(peaksNUMatrix[l][NUsec[p+1]][9]!=0)){
				for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){
					for(int j=0; j<8; j++){
						if (peaksNUMatrix[l][i][j+3]>peaksNUMax[p][l][j+3]){
							pmtCounter++;
						}
					}
					if(pmtCounter>4){
						for(int k=1; k<12; k++){
							peaksNUMax[p][l][k]=peaksNUMatrix[l][i][k];
						}
					}
					pmtCounter=0;
				}
			}
		}
	}
	for (int p=0; p<=ndsec; p++){ //Repeat for Nikko Down
		for(int l=1; l<=cynd; l++){
			if((peaksNDMatrix[l][NDsec[p+1]][3]!=0)and(peaksNDMatrix[l][NDsec[p+1]][6]!=0)and(peaksNDMatrix[l][NDsec[p+1]][9]!=0)){
				for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){
					for(int j=0; j<8; j++){
						if (peaksNDMatrix[l][i][j+3]>peaksNDMax[p][l][j+3]){
							pmtCounter++;
						}
					}
					if(pmtCounter>4){
						for(int k=1; k<12; k++){
							peaksNDMax[p][l][k]=peaksNDMatrix[l][i][k];
						}
					}
					pmtCounter=0;
				}
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This part is to find the steps number jump for each peak for each area

	for (int p=0; p<=ousec; p++){ //For each area
		for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){ //And each peak in that area
			motor1r[p]=abs(peaksOU[i][1]-peaksOU[i+1][1]); //Save the diff of motor position between that peak and the next
			if (motor1r[p]!=0) break; //If the diff is not zero, thats the value we need
		}
		for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){ //Same for the other motor
			motor2r[p]=abs(peaksOU[i][2]-peaksOU[i+1][2]);
			if (motor2r[p]!=0) break;
		}
		if(motor1r[p]==0) motor1r[p]=1; //If for whatever reason the diff ends up zero, then we make it 1
		if(motor2r[p]==0) motor2r[p]=1;
	}
	for (int p=0; p<=odsec; p++){ //Repeat for Oho Down
		for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){
			motor3r[p]=abs(peaksOD[i][1]-peaksOD[i+1][1]);
			if (motor3r[p]!=0) break;
		}
		for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){
			motor4r[p]=abs(peaksOD[i][2]-peaksOD[i+1][2]);
			if (motor4r[p]!=0) break;
		}
		if(motor3r[p]==0) motor3r[p]=1;
		if(motor4r[p]==0) motor4r[p]=1;
	}
	for (int p=0; p<=ndsec; p++){ //Repeat for Nikko Down
		for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){
			motor11r[p]=abs(peaksND[i][1]-peaksND[i+1][1]);
			if (motor11r[p]!=0) break;
		}
		for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){
			motor12r[p]=abs(peaksND[i][2]-peaksND[i+1][2]);
			if (motor12r[p]!=0) break;
		}
		if(motor11r[p]==0) motor11r[p]=1;
		if(motor12r[p]==0) motor12r[p]=1;
	}
	for (int p=0; p<=nusec; p++){ //Repeat for Nikko Up
		for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){
			motor9r[p]=abs(peaksNU[i][1]-peaksNU[i+1][1]);
			if (motor9r[p]!=0) break;
		}
		for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){
			motor10r[p]=abs(peaksNU[i][2]-peaksNU[i+1][2]);
			if (motor10r[p]!=0) break;
		}
		if(motor9r[p]==0) motor9r[p]=1;
		if(motor10r[p]==0) motor10r[p]=1;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Find the triplet points for the highest rate position

//////Oho Up Triplet
for (int p=0; p<=ousec; p++){ //For each area
	for(int l=1; l<=cyou; l++){ //For each cycle
		if (peaksOUMax[p][l][1] == peaksOU[OUsec[p]+1][1]){ //If the Max rate peak is at the left edge
			for (int j=1; j<=12; j++){
				r1[p][l][j] = peaksOUMax[p][l][j]; //That's r1
			}
			for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){ //For each peak we search for the peak thats 1 point to the right
			if((peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1]+motor1r[p])and(peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2])){ 
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksOUMatrix[l][i][j]; //That's r2
				}
			} //For each peak we search for the peak thats 2 points to the right
			if((peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1]+(2*motor1r[p]))and(peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksOUMatrix[l][i][j]; //That's r3
				}
			}
			}
		}else if (peaksOUMax[p][l][1] == peaksOU[OUsec[p+1]][1]){ //If the Max rate peak is at the right edge
			for (int j=1; j<=12; j++){
				r3[p][l][j] = peaksOUMax[p][l][j]; //That's r3
			}
			for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){//For each peak we search for the peak thats 1 point to the left
			if ((peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1]-motor1r[p]) and (peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksOUMatrix[l][i][j]; //That's r2
				}
			}//For each peak we search for the peak thats 2 points to the left
			if ((peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1]-(2*motor1r[p])) and (peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksOUMatrix[l][i][j]; //That's r1
				}
			}
			}
		}else{ //If the Max is not on any edge
			for (int j=1; j<=12; j++){
				r2[p][l][j] = peaksOUMax[p][l][j]; //The max is r2
			}
			for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){ //For each peak we search for the peak thats 1 point to the left
			if ((peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1]-motor1r[p]) and (peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksOUMatrix[l][i][j]; //That's r1
				}
			}//For each peak we search for the peak thats 1 point to the right
			if ((peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1]+motor1r[p]) and (peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksOUMatrix[l][i][j]; //That's r3
				}
			}
			}
		}
		if (r2[p][l][1]==0){ //This happens only if for whatever reason the Max peak is 0, and its to prevent issues
			for(int j=1; j<=8; j++){
 				r2[p][l][j+2]=1;
				r1[p][l][j+2]=0;
				r3[p][l][j+2]=0;
			}
		}
		for(int j=1; j<=8; j++){ //Calculate the tripple point of the first motor for each pmt
		OUPMTMax[p][l][1][j]=r2[p][l][1]+(motor1r[p]*(r1[p][l][j+2]-r3[p][l][j+2])/(r1[p][l][j+2]-(2*r2[p][l][j+2])+r3[p][l][j+2]));
		}
	}
	for(int l=1; l<=cyou; l++){ //Repeat for the second motor of Oho Up
		if (peaksOUMax[p][l][2] == peaksOU[OUsec[p]+1][2]){
			for (int j=1; j<=12; j++){
				r1[p][l][j] = peaksOUMax[p][l][j];
			}
			for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){ 
			if((peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2]+motor2r[p])and(peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1])){ 
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksOUMatrix[l][i][j];
				}
			}
			if((peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2]+(2*motor2r[p]))and(peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksOUMatrix[l][i][j];
				}
			}
			}
		}else if (peaksOUMax[p][l][2] == peaksOU[OUsec[p+1]][2]){
			for (int j=1; j<=12; j++){
				r3[p][l][j] = peaksOUMax[p][l][j];
			}
			for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){
			if ((peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2]-motor2r[p]) and (peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksOUMatrix[l][i][j];
				}
			}
			if ((peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2]-(2*motor2r[p])) and (peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksOUMatrix[l][i][j];
				}
			}
			}
		}else{
			for (int j=1; j<=12; j++){
				r2[p][l][j] = peaksOUMax[p][l][j];
			}
			for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){
			if ((peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2]-motor2r[p]) and (peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksOUMatrix[l][i][j];
				}
			}
			if ((peaksOUMatrix[l][i][2]==peaksOUMax[p][l][2]+motor2r[p]) and (peaksOUMatrix[l][i][1]==peaksOUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksOUMatrix[l][i][j];
				}
			}
			}
		}
		if (r2[p][l][2]==0){
			for(int j=1; j<=8; j++){
 				r2[p][l][j+2]=1;
				r1[p][l][j+2]=0;
				r3[p][l][j+2]=0;
			}
		}
		for(int j=1; j<=8; j++){	
		OUPMTMax[p][l][2][j]=r2[p][l][2]+(motor2r[p]*(r1[p][l][j+2]-r3[p][l][j+2])/(r1[p][l][j+2]-(2*r2[p][l][j+2])+r3[p][l][j+2]));
		}
	}
}

//////Oho Down Triplet
for (int p=0; p<=odsec; p++){//Repeat for Oho Down
	for(int l=1; l<=cyod; l++){
		if (peaksODMax[p][l][1] == peaksOD[ODsec[p]+1][1]){
			for (int j=1; j<=12; j++){
				r1[p][l][j] = peaksODMax[p][l][j];
			}
			for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){ 
			if((peaksODMatrix[l][i][1]==peaksODMax[p][l][1]+motor3r[p])and(peaksODMatrix[l][i][2]==peaksODMax[p][l][2])){ 
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			if((peaksODMatrix[l][i][1]==peaksODMax[p][l][1]+(2*motor3r[p]))and(peaksODMatrix[l][i][2]==peaksODMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			}
		}else if (peaksODMax[p][l][1] == peaksOD[ODsec[p+1]][1]){
			for (int j=1; j<=12; j++){
				r3[p][l][j] = peaksODMax[p][l][j];
			}
			for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){
			if ((peaksODMatrix[l][i][1]==peaksODMax[p][l][1]-motor3r[p]) and (peaksODMatrix[l][i][2]==peaksODMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			if ((peaksODMatrix[l][i][1]==peaksODMax[p][l][1]-(2*motor3r[p])) and (peaksODMatrix[l][i][2]==peaksODMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			}
		}else{
			for (int j=1; j<=12; j++){
				r2[p][l][j] = peaksODMax[p][l][j];
			}
			for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){
			if ((peaksODMatrix[l][i][1]==peaksODMax[p][l][1]-motor3r[p]) and (peaksODMatrix[l][i][2]==peaksODMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			if ((peaksODMatrix[l][i][1]==peaksODMax[p][l][1]+motor3r[p]) and (peaksODMatrix[l][i][2]==peaksODMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			}
		}
		if (r2[p][l][1]==0){
			for(int j=1; j<=8; j++){
 				r2[p][l][j+2]=1;
				r1[p][l][j+2]=0;
				r3[p][l][j+2]=0;
			}
		}
		for(int j=1; j<=8; j++){	
		ODPMTMax[p][l][1][j]=r2[p][l][1]+(motor3r[p]*(r1[p][l][j+2]-r3[p][l][j+2])/(r1[p][l][j+2]-(2*r2[p][l][j+2])+r3[p][l][j+2]));
		}
	}
	for(int l=1; l<=cyod; l++){
		if (peaksODMax[p][l][2] == peaksOD[ODsec[p]+1][2]){
			for (int j=1; j<=12; j++){
				r1[p][l][j] = peaksODMax[p][l][j];
			}
			for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){ 
			if((peaksODMatrix[l][i][2]==peaksODMax[p][l][2]+motor4r[p])and(peaksODMatrix[l][i][1]==peaksODMax[p][l][1])){ 
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			if((peaksODMatrix[l][i][2]==peaksODMax[p][l][2]+(2*motor4r[p]))and(peaksODMatrix[l][i][1]==peaksODMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			}
		}else if (peaksODMax[p][l][2] == peaksOD[ODsec[p+1]][2]){
			for (int j=1; j<=12; j++){
				r3[p][l][j] = peaksODMax[p][l][j];
			}
			for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){
			if ((peaksODMatrix[l][i][2]==peaksODMax[p][l][2]-motor4r[p]) and (peaksODMatrix[l][i][1]==peaksODMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			if ((peaksODMatrix[l][i][2]==peaksODMax[p][l][2]-(2*motor4r[p])) and (peaksODMatrix[l][i][1]==peaksODMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			}
		}else{
			for (int j=1; j<=12; j++){
				r2[p][l][j] = peaksODMax[p][l][j];
			}
			for (int i=(ODsec[p]+1); i<=ODsec[p+1]; i++){
			if ((peaksODMatrix[l][i][2]==peaksODMax[p][l][2]-motor4r[p]) and (peaksODMatrix[l][i][1]==peaksODMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			if ((peaksODMatrix[l][i][2]==peaksODMax[p][l][2]+motor4r[p]) and (peaksODMatrix[l][i][1]==peaksODMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksODMatrix[l][i][j];
				}
			}
			}
		}
		if (r2[p][l][2]==0){
			for(int j=1; j<=8; j++){
 				r2[p][l][j+2]=1;
				r1[p][l][j+2]=0;
				r3[p][l][j+2]=0;
			}
		}
		for(int j=1; j<=8; j++){	
		ODPMTMax[p][l][2][j]=r2[p][l][2]+(motor4r[p]*(r1[p][l][j+2]-r3[p][l][j+2])/(r1[p][l][j+2]-(2*r2[p][l][j+2])+r3[p][l][j+2]));
		}
	}
}

//////Nikko Up Triplet
for (int p=0; p<=nusec; p++){//Repeat for Nikko Up
	for(int l=1; l<=cynu; l++){
		if (peaksNUMax[p][l][1] == peaksNU[NUsec[p]+1][1]){
			for (int j=1; j<=12; j++){
				r1[p][l][j] = peaksNUMax[p][l][j];
			}
			for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){ 
			if((peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1]+motor9r[p])and(peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2])){ 
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			if((peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1]+(2*motor9r[p]))and(peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			}
		}else if (peaksNUMax[p][l][1] == peaksNU[NUsec[p+1]][1]){
			for (int j=1; j<=12; j++){
				r3[p][l][j] = peaksNUMax[p][l][j];
			}
			for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){
			if ((peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1]-motor9r[p]) and (peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			if ((peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1]-(2*motor9r[p])) and (peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			}
		}else{
			for (int j=1; j<=12; j++){
				r2[p][l][j] = peaksNUMax[p][l][j];
			}
			for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){
			if ((peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1]-motor9r[p]) and (peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			if ((peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1]+motor9r[p]) and (peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			}
		}
		if (r2[p][l][1]==0){
			for(int j=1; j<=8; j++){
 				r2[p][l][j+2]=1;
				r1[p][l][j+2]=0;
				r3[p][l][j+2]=0;
			}
		}
		for(int j=1; j<=8; j++){	
		NUPMTMax[p][l][1][j]=r2[p][l][1]+(motor9r[p]*(r1[p][l][j+2]-r3[p][l][j+2])/(r1[p][l][j+2]-(2*r2[p][l][j+2])+r3[p][l][j+2]));
		}
	}
	for(int l=1; l<=cynu; l++){
		if (peaksNUMax[p][l][2] == peaksNU[NUsec[p]+1][2]){
			for (int j=1; j<=12; j++){
				r1[p][l][j] = peaksNUMax[p][l][j];
			}
			for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){ 
			if((peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2]+motor10r[p])and(peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1])){ 
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			if((peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2]+(2*motor10r[p]))and(peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			}
		}else if (peaksNUMax[p][l][2] == peaksNU[NUsec[p+1]][2]){
			for (int j=1; j<=12; j++){
				r3[p][l][j] = peaksNUMax[p][l][j];
			}
			for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){
			if ((peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2]-motor10r[p]) and (peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			if ((peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2]-(2*motor10r[p])) and (peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			}
		}else{
			for (int j=1; j<=12; j++){
				r2[p][l][j] = peaksNUMax[p][l][j];
			}
			for (int i=(NUsec[p]+1); i<=NUsec[p+1]; i++){
			if ((peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2]-motor10r[p]) and (peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			if ((peaksNUMatrix[l][i][2]==peaksNUMax[p][l][2]+motor10r[p]) and (peaksNUMatrix[l][i][1]==peaksNUMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksNUMatrix[l][i][j];
				}
			}
			}
		}
		if (r2[p][l][2]==0){
			for(int j=1; j<=8; j++){
 				r2[p][l][j+2]=1;
				r1[p][l][j+2]=0;
				r3[p][l][j+2]=0;
			}
		}
		for(int j=1; j<=8; j++){	
		NUPMTMax[p][l][2][j]=r2[p][l][2]+(motor10r[p]*(r1[p][l][j+2]-r3[p][l][j+2])/(r1[p][l][j+2]-(2*r2[p][l][j+2])+r3[p][l][j+2]));
		}
	}
}

//////Nikko Down Triplet
for (int p=0; p<=ndsec; p++){ //Repeat for Nikko Down
	for(int l=1; l<=cynd; l++){
		if (peaksNDMax[p][l][1] == peaksND[NDsec[p]+1][1]){
			for (int j=1; j<=12; j++){
				r1[p][l][j] = peaksNDMax[p][l][j];
			}
			for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){ 
			if((peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1]+motor11r[p])and(peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2])){ 
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			if((peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1]+(2*motor11r[p]))and(peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			}
		}else if (peaksNDMax[p][l][1] == peaksND[NDsec[p+1]][1]){
			for (int j=1; j<=12; j++){
				r3[p][l][j] = peaksNDMax[p][l][j];
			}
			for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){
			if ((peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1]-motor11r[p]) and (peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			if ((peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1]-(2*motor11r[p])) and (peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			}
		}else{
			for (int j=1; j<=12; j++){
				r2[p][l][j] = peaksNDMax[p][l][j];
			}
			for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){
			if ((peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1]-motor11r[p]) and (peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			if ((peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1]+motor11r[p]) and (peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			}
		}
		if (r2[p][l][1]==0){
			for(int j=1; j<=8; j++){
 				r2[p][l][j+2]=1;
				r1[p][l][j+2]=0;
				r3[p][l][j+2]=0;
			}
		}
		for(int j=1; j<=8; j++){	
		NDPMTMax[p][l][1][j]=r2[p][l][1]+(motor11r[p]*(r1[p][l][j+2]-r3[p][l][j+2])/(r1[p][l][j+2]-(2*r2[p][l][j+2])+r3[p][l][j+2]));
		}
	}
	for(int l=1; l<=cynd; l++){
		if (peaksNDMax[p][l][2] == peaksND[NDsec[p]+1][2]){
			for (int j=1; j<=12; j++){
				r1[p][l][j] = peaksNDMax[p][l][j];
			}
			for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){ 
			if((peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2]+motor12r[p])and(peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1])){ 
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			if((peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2]+(2*motor12r[p]))and(peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			}
		}else if (peaksNDMax[p][l][2] == peaksND[NDsec[p+1]][2]){
			for (int j=1; j<=12; j++){
				r3[p][l][j] = peaksNDMax[p][l][j];
			}
			for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){
			if ((peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2]-motor12r[p]) and (peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r2[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			if ((peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2]-(2*motor12r[p])) and (peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			}
		}else{
			for (int j=1; j<=12; j++){
				r2[p][l][j] = peaksNDMax[p][l][j];
			}
			for (int i=(NDsec[p]+1); i<=NDsec[p+1]; i++){
			if ((peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2]-motor12r[p]) and (peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r1[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			if ((peaksNDMatrix[l][i][2]==peaksNDMax[p][l][2]+motor12r[p]) and (peaksNDMatrix[l][i][1]==peaksNDMax[p][l][1])){
				for (int j=1; j<=12; j++){
					r3[p][l][j] = peaksNDMatrix[l][i][j];
				}
			}
			}
		}
		if (r2[p][l][2]==0){
			for(int j=1; j<=8; j++){
 				r2[p][l][j+2]=1;
				r1[p][l][j+2]=0;
				r3[p][l][j+2]=0;
			}
		}
		for(int j=1; j<=8; j++){	
		NDPMTMax[p][l][2][j]=r2[p][l][2]+(motor12r[p]*(r1[p][l][j+2]-r3[p][l][j+2])/(r1[p][l][j+2]-(2*r2[p][l][j+2])+r3[p][l][j+2]));
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The next part of the code is to create data quality graphs and print them

	TCanvas c3;
	c3.Divide(1,2);

	TCanvas c4;
	c4.Divide(1,0);

	PMTMax1=0;
	PMTMin1=0;
	PMTMax2=0;
	PMTMin2=0;

	if(ou!=0){
		for (int p=0; p<=ousec; p++){

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cyou; l++){
				if (OUPMTMax[p][l][1][1]!=0){
					if (PMTMax1==0) PMTMax1=OUPMTMax[p][l][1][1];
					if (OUPMTMax[p][l][1][1]>PMTMax1){
						PMTMax1=OUPMTMax[p][l][1][1];
					}
					if (PMTMin1==0) PMTMin1=OUPMTMax[p][l][1][1];
					if (PMTMin1>OUPMTMax[p][l][1][1]){
						PMTMin1=OUPMTMax[p][l][1][1];
					}
				}
				if (OUPMTMax[p][l][2][1]!=0){
					if (PMTMax2==0) PMTMax2=OUPMTMax[p][l][2][1];
					if (OUPMTMax[p][l][2][1]>PMTMax2){
						PMTMax2=OUPMTMax[p][l][2][1];
					}
					if (PMTMin2==0) PMTMin2=OUPMTMax[p][l][2][1];
					if (PMTMin2>OUPMTMax[p][l][2][1]){
						PMTMin2=OUPMTMax[p][l][2][1];
					}
				}
			}

			PMTMax1=PMTMax1 + motor1r[p];
			PMTMin1=PMTMin1 - motor1r[p];
			PMTMax2=PMTMax2 + motor2r[p];
			PMTMin2=PMTMin2 - motor2r[p];

			TH2* PMT1MaxMotor1 = new TH2F("PMT1 Max Motor1", "PMT1 Max peak Motor 1 change over time ", cyou, 0, cyou, cyou, PMTMin1, PMTMax1);
			PMT1MaxMotor1->SetStats(0);
			PMT1MaxMotor1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
			PMT1MaxMotor1->GetYaxis()->SetTitle("PMT1 Motor 1 Max Peak position");

			TH2* PMT1MaxMotor2 = new TH2F("PMT1 Max Motor2", "PMT1 Max peak Motor 2 change over time ", cyou, 0, cyou, cyou, PMTMin2, PMTMax2);
			PMT1MaxMotor2->SetStats(0);
			PMT1MaxMotor2->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
			PMT1MaxMotor2->GetYaxis()->SetTitle("PMT1 Motor 2 Max Peak position");

			for(int l=1; l<=cyou; l++){
				PMT1MaxMotor1->Fill(l, OUPMTMax[p][l][1][1]);
				PMT1MaxMotor2->Fill(l, OUPMTMax[p][l][2][1]);
			}

			c3.cd(1);
			PMT1MaxMotor1->Draw("");
			c3.cd(2);
			PMT1MaxMotor2->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/PMT1MaxPeakPositionChangeRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c3.SaveAs(nombrepdf.c_str());

			delete PMT1MaxMotor1;
			delete PMT1MaxMotor2;

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cyou; l++){
				if (peaksOUMax[p][l][1]!=0){
					if (PMTMax1==0) PMTMax1=peaksOUMax[p][l][1];
					if (peaksOUMax[p][l][1]>PMTMax1){
						PMTMax1=peaksOUMax[p][l][1];
					}
					if (PMTMin1==0) PMTMin1=peaksOUMax[p][l][1];
					if (PMTMin1>peaksOUMax[p][l][1]){
						PMTMin1=peaksOUMax[p][l][1];
					}
				}
				if (peaksOUMax[p][l][2]!=0){
					if (PMTMax2==0) PMTMax2=peaksOUMax[p][l][2];
					if (peaksOUMax[p][l][2]>PMTMax2){
						PMTMax2=peaksOUMax[p][l][2];
					}
					if (PMTMin2==0) PMTMin2=peaksOUMax[p][l][2];
					if (PMTMin2>peaksOUMax[p][l][2]){
						PMTMin2=peaksOUMax[p][l][2];
					}
				}
			}

			PMTMax1=PMTMax1 + motor1r[p];
			PMTMin1=PMTMin1 - motor1r[p];
			PMTMax2=PMTMax2 + motor2r[p];
			PMTMin2=PMTMin2 - motor2r[p];

			TH2* MaxMotor1 = new TH2F("Max Motor1", "Max peak Motor 1 change over time ", cyou, 0, cyou, cyou, PMTMin1, PMTMax1);
			MaxMotor1->SetStats(0);
			MaxMotor1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
			MaxMotor1->GetYaxis()->SetTitle("Motor 1 Max Peak position");

			TH2* MaxMotor2 = new TH2F("Max Motor2", "Max peak Motor 2 change over time ", cyou, 0, cyou, cyou, PMTMin2, PMTMax2);
			MaxMotor2->SetStats(0);
			MaxMotor2->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
			MaxMotor2->GetYaxis()->SetTitle("Motor 2 Max Peak position");

			for(int l=1; l<=cyou; l++){
				MaxMotor1->Fill(l, peaksOUMax[p][l][1]);
				MaxMotor2->Fill(l, peaksOUMax[p][l][2]);
			}

			c3.cd(1);
			MaxMotor1->Draw("");
			c3.cd(2);
			MaxMotor2->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/Motor1Motor2MaxPeakPositionChangeRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c3.SaveAs(nombrepdf.c_str());

			delete MaxMotor1;
			delete MaxMotor2;
			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cyou; l++){
				if ((peaksOUMax[p][l][3]!=0) and (peaksOUMax[p][l][10]!=0)){
					if (PMTMax1==0) PMTMax1=peaksOUMax[p][l][10]/peaksOUMax[p][l][3];
					if ((peaksOUMax[p][l][10]/peaksOUMax[p][l][3])>PMTMax1){
						PMTMax1=peaksOUMax[p][l][10]/peaksOUMax[p][l][3];
					}
					if (PMTMin1==0) PMTMin1=peaksOUMax[p][l][10]/peaksOUMax[p][l][3];
					if (PMTMin1>(peaksOUMax[p][l][10]/peaksOUMax[p][l][3])){
						PMTMin1=peaksOUMax[p][l][10]/peaksOUMax[p][l][3];
					}
				}
			}

			TH2* PMT8O1 = new TH2F("PMT8 over PMT1", "PMT8 Over PMT1 of Max Peak ", cyou, 0, cyou, cyou, PMTMin1, PMTMax1);
			PMT8O1->SetStats(0);
			PMT8O1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
			PMT8O1->GetYaxis()->SetTitle("PMT8/PMT1 of Max Peak");

			for(int l=1; l<=cyou; l++){
				PMT8O1->Fill(l, peaksOUMax[p][l][10]/peaksOUMax[p][l][3]);
			}

			c4.cd(1);
			PMT8O1->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/PMT8overPMT1MaxPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c4.SaveAs(nombrepdf.c_str());

			delete PMT8O1;
			PMTMax1=0;
			PMTMin1=0;
		}
	}
	if(od!=0){
		for (int p=0; p<=odsec; p++){

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cyod; l++){
				if (ODPMTMax[p][l][1][1]!=0){
					if (PMTMax1==0) PMTMax1=ODPMTMax[p][l][1][1];
					if (ODPMTMax[p][l][1][1]>PMTMax1){
						PMTMax1=ODPMTMax[p][l][1][1];
					}
					if (PMTMin1==0) PMTMin1=ODPMTMax[p][l][1][1];
					if (PMTMin1>ODPMTMax[p][l][1][1]){
						PMTMin1=ODPMTMax[p][l][1][1];
					}
				}
				if (ODPMTMax[p][l][2][1]!=0){
					if (PMTMax2==0) PMTMax2=ODPMTMax[p][l][2][1];
					if (ODPMTMax[p][l][2][1]>PMTMax2){
						PMTMax2=ODPMTMax[p][l][2][1];
					}
					if (PMTMin2==0) PMTMin2=ODPMTMax[p][l][2][1];
					if (PMTMin2>ODPMTMax[p][l][2][1]){
						PMTMin2=ODPMTMax[p][l][2][1];
					}
				}
			}

			PMTMax1=PMTMax1 + motor3r[p];
			PMTMin1=PMTMin1 - motor3r[p];
			PMTMax2=PMTMax2 + motor4r[p];
			PMTMin2=PMTMin2 - motor4r[p];

			TH2* PMT9MaxMotor3 = new TH2F("PMT9 Max Motor3", "PMT9 Max peak Motor 3 change over time ", cyod, 0, cyod, cyod, PMTMin1, PMTMax1);
			PMT9MaxMotor3->SetStats(0);
			PMT9MaxMotor3->GetXaxis()->SetTitle("Oho Down Peak Cycle (one cycle is 5 seconds)");
			PMT9MaxMotor3->GetYaxis()->SetTitle("PMT9 Motor 3 Max Peak position");

			TH2* PMT9MaxMotor4 = new TH2F("PMT9 Max Motor4", "PMT9 Max peak Motor 4 change over time ", cyod, 0, cyod, cyod, PMTMin2, PMTMax2);
			PMT9MaxMotor4->SetStats(0);
			PMT9MaxMotor4->GetXaxis()->SetTitle("Oho Down Peak Cycle (one cycle is 5 seconds)");
			PMT9MaxMotor4->GetYaxis()->SetTitle("PMT9 Motor 4 Max Peak position");

			for(int l=1; l<=cyod; l++){
				PMT9MaxMotor3->Fill(l, ODPMTMax[p][l][1][1]);
				PMT9MaxMotor4->Fill(l, ODPMTMax[p][l][2][1]);
			}

			c3.cd(1);
			PMT9MaxMotor3->Draw("");
			c3.cd(2);
			PMT9MaxMotor4->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/PMT9MaxPeakPositionChangeRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c3.SaveAs(nombrepdf.c_str());

			delete PMT9MaxMotor3;
			delete PMT9MaxMotor4;
			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cyod; l++){
				if (peaksODMax[p][l][1]!=0){
					if (PMTMax1==0) PMTMax1=peaksODMax[p][l][1];
					if (peaksODMax[p][l][1]>PMTMax1){
						PMTMax1=peaksODMax[p][l][1];
					}
					if (PMTMin1==0) PMTMin1=peaksODMax[p][l][1];
					if (PMTMin1>peaksODMax[p][l][1]){
						PMTMin1=peaksODMax[p][l][1];
					}
				}
				if (peaksODMax[p][l][2]!=0){
					if (PMTMax2==0) PMTMax2=peaksODMax[p][l][2];
					if (peaksODMax[p][l][2]>PMTMax2){
						PMTMax2=peaksODMax[p][l][2];
					}
					if (PMTMin2==0) PMTMin2=peaksODMax[p][l][2];
					if (PMTMin2>peaksODMax[p][l][2]){
						PMTMin2=peaksODMax[p][l][2];
					}
				}
			}

			PMTMax1=PMTMax1 + motor3r[p];
			PMTMin1=PMTMin1 - motor3r[p];
			PMTMax2=PMTMax2 + motor4r[p];
			PMTMin2=PMTMin2 - motor4r[p];

			TH2* MaxMotor3 = new TH2F("Max Motor3", "Max peak Motor 3 change over time ", cyod, 0, cyod, cyod, PMTMin1, PMTMax1);
			MaxMotor3->SetStats(0);
			MaxMotor3->GetXaxis()->SetTitle("Oho Down Peak Cycle (one cycle is 5 seconds)");
			MaxMotor3->GetYaxis()->SetTitle("Motor 3 Max Peak position");

			TH2* MaxMotor4 = new TH2F("Max Motor4", "Max peak Motor 4 change over time ", cyod, 0, cyod, cyod, PMTMin2, PMTMax2);
			MaxMotor4->SetStats(0);
			MaxMotor4->GetXaxis()->SetTitle("Oho Down Peak Cycle (one cycle is 5 seconds)");
			MaxMotor4->GetYaxis()->SetTitle("Motor 4 Max Peak position");

			for(int l=1; l<=cyod; l++){
				MaxMotor3->Fill(l, peaksODMax[p][l][1]);
				MaxMotor4->Fill(l, peaksODMax[p][l][2]);
			}

			c3.cd(1);
			MaxMotor3->Draw("");
			c3.cd(2);
			MaxMotor4->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/Motor3Motor4MaxPeakPositionChangeRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c3.SaveAs(nombrepdf.c_str());

			delete MaxMotor3;
			delete MaxMotor4;
			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cyod; l++){
				if ((peaksODMax[p][l][3]!=0) and (peaksODMax[p][l][10]!=0)){
					if (PMTMax1==0) PMTMax1=peaksODMax[p][l][10]/peaksODMax[p][l][3];
					if ((peaksODMax[p][l][10]/peaksODMax[p][l][3])>PMTMax1){
						PMTMax1=peaksODMax[p][l][10]/peaksODMax[p][l][3];
					}
					if (PMTMin1==0) PMTMin1=peaksODMax[p][l][10]/peaksODMax[p][l][3];
					if (PMTMin1>(peaksODMax[p][l][10]/peaksODMax[p][l][3])){
						PMTMin1=peaksODMax[p][l][10]/peaksODMax[p][l][3];
					}
				}
			}

			TH2* PMT9O16 = new TH2F("PMT9 over PMT16", "PMT9 Over PMT16 of Max Peak ", cyod, 0, cyod, cyod, PMTMin1, PMTMax1);
			PMT9O16->SetStats(0);
			PMT9O16->GetXaxis()->SetTitle("Oho Down Peak Cycle (one cycle is 5 seconds)");
			PMT9O16->GetYaxis()->SetTitle("PMT9/PMT16 of Max Peak");

			for(int l=1; l<=cyod; l++){
				PMT9O16->Fill(l, peaksODMax[p][l][10]/peaksODMax[p][l][3]);
			}

			c4.cd(1);
			PMT9O16->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/PMT9overPMT16MaxPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c4.SaveAs(nombrepdf.c_str());

			delete PMT9O16;
			PMTMax1=0;
			PMTMin1=0;


		}
	}
	if(nd!=0){
		for (int p=0; p<=ndsec; p++){

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cynd; l++){
				if (NDPMTMax[p][l][1][1]!=0){
					if (PMTMax1==0) PMTMax1=NDPMTMax[p][l][1][1];
					if (NDPMTMax[p][l][1][1]>PMTMax1){
						PMTMax1=NDPMTMax[p][l][1][1];
					}
					if (PMTMin1==0) PMTMin1=NDPMTMax[p][l][1][1];
					if (PMTMin1>NDPMTMax[p][l][1][1]){
						PMTMin1=NDPMTMax[p][l][1][1];
					}
				}
				if (NDPMTMax[p][l][2][1]!=0){
					if (PMTMax2==0) PMTMax2=NDPMTMax[p][l][2][1];
					if (NDPMTMax[p][l][2][1]>PMTMax2){
						PMTMax2=NDPMTMax[p][l][2][1];
					}
					if (PMTMin2==0) PMTMin2=NDPMTMax[p][l][2][1];
					if (PMTMin2>NDPMTMax[p][l][2][1]){
						PMTMin2=NDPMTMax[p][l][2][1];
					}
				}
			}

			PMTMax1=PMTMax1 + motor11r[p];
			PMTMin1=PMTMin1 - motor11r[p];
			PMTMax2=PMTMax2 + motor12r[p];
			PMTMin2=PMTMin2 - motor12r[p];

			TH2* PMT17MaxMotor11 = new TH2F("PMT17 Max Motor11", "PMT17 Max peak Motor 11 change over time ", cynd, 0, cynd, cynd, PMTMin1, PMTMax1);
			PMT17MaxMotor11->SetStats(0);
			PMT17MaxMotor11->GetXaxis()->SetTitle("Nikko Down Peak Cycle (one cycle is 5 seconds)");
			PMT17MaxMotor11->GetYaxis()->SetTitle("PMT17 Motor 11 Max Peak position");

			TH2* PMT17MaxMotor12 = new TH2F("PMT17 Max Motor12", "PMT17 Max peak Motor 12 change over time ", cynd, 0, cynd, cynd, PMTMin2, PMTMax2);
			PMT17MaxMotor12->SetStats(0);
			PMT17MaxMotor12->GetXaxis()->SetTitle("Nikko Down Peak Cycle (one cycle is 5 seconds)");
			PMT17MaxMotor12->GetYaxis()->SetTitle("PMT17 Motor 12 Max Peak position");

			for(int l=1; l<=cynd; l++){
				PMT17MaxMotor11->Fill(l, NDPMTMax[p][l][1][1]);
				PMT17MaxMotor12->Fill(l, NDPMTMax[p][l][2][1]);
			}

			c3.cd(1);
			PMT17MaxMotor11->Draw("");
			c3.cd(2);
			PMT17MaxMotor12->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/PMT17MaxPeakPositionChangeRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c3.SaveAs(nombrepdf.c_str());

			delete PMT17MaxMotor11;
			delete PMT17MaxMotor12;
			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cynd; l++){
				if (peaksNDMax[p][l][1]!=0){
					if (PMTMax1==0) PMTMax1=peaksNDMax[p][l][1];
					if (peaksNDMax[p][l][1]>PMTMax1){
						PMTMax1=peaksNDMax[p][l][1];
					}
					if (PMTMin1==0) PMTMin1=peaksNDMax[p][l][1];
					if (PMTMin1>peaksNDMax[p][l][1]){
						PMTMin1=peaksNDMax[p][l][1];
					}
				}
				if (peaksNDMax[p][l][2]!=0){
					if (PMTMax2==0) PMTMax2=peaksNDMax[p][l][2];
					if (peaksNDMax[p][l][2]>PMTMax2){
						PMTMax2=peaksNDMax[p][l][2];
					}
					if (PMTMin2==0) PMTMin2=peaksNDMax[p][l][2];
					if (PMTMin2>peaksNDMax[p][l][2]){
						PMTMin2=peaksNDMax[p][l][2];
					}
				}
			}

			PMTMax1=PMTMax1 + motor11r[p];
			PMTMin1=PMTMin1 - motor11r[p];
			PMTMax2=PMTMax2 + motor12r[p];
			PMTMin2=PMTMin2 - motor12r[p];

			TH2* MaxMotor11 = new TH2F("Max Motor11", "Max peak Motor 11 change over time", cynd, 0, cynd, cynd, PMTMin1, PMTMax1);
			MaxMotor11->SetStats(0);
			MaxMotor11->GetXaxis()->SetTitle("Nikko Down Peak Cycle (one cycle is 5 seconds)");
			MaxMotor11->GetYaxis()->SetTitle("Motor 11 Max Peak position");

			TH2* MaxMotor12 = new TH2F("Max Motor12", "Max peak Motor 12 change over time", cynd, 0, cynd, cynd, PMTMin2, PMTMax2);
			MaxMotor12->SetStats(0);
			MaxMotor12->GetXaxis()->SetTitle("Nikko Down Peak Cycle (one cycle is 5 seconds)");
			MaxMotor12->GetYaxis()->SetTitle("Motor 12 Max Peak position");

			for(int l=1; l<=cynd; l++){
				MaxMotor11->Fill(l, peaksNDMax[p][l][1]);
				MaxMotor12->Fill(l, peaksNDMax[p][l][2]);
			}

			c3.cd(1);
			MaxMotor11->Draw("");
			c3.cd(2);
			MaxMotor12->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/Motor11Motor12MaxPeakPositionChangeRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c3.SaveAs(nombrepdf.c_str());

			delete MaxMotor11;
			delete MaxMotor12;
			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cynd; l++){
				if ((peaksNDMax[p][l][3]!=0) and (peaksNDMax[p][l][10]!=0)){
					if (PMTMax1==0) PMTMax1=peaksNDMax[p][l][10]/peaksNDMax[p][l][3];
					if ((peaksNDMax[p][l][10]/peaksNDMax[p][l][3])>PMTMax1){
						PMTMax1=peaksNDMax[p][l][10]/peaksNDMax[p][l][3];
					}
					if (PMTMin1==0) PMTMin1=peaksNDMax[p][l][10]/peaksNDMax[p][l][3];
					if (PMTMin1>(peaksNDMax[p][l][10]/peaksNDMax[p][l][3])){
						PMTMin1=peaksNDMax[p][l][10]/peaksNDMax[p][l][3];
					}
				}
			}

			TH2* PMT17O24 = new TH2F("PMT17 over PMT24", "PMT17 Over PMT24 of Max Peak ", cynd, 0, cynd, cynd, PMTMin1, PMTMax1);
			PMT17O24->SetStats(0);
			PMT17O24->GetXaxis()->SetTitle("Nikko Down Peak Cycle (one cycle is 5 seconds)");
			PMT17O24->GetYaxis()->SetTitle("PMT17/PMT24 of Max Peak");

			for(int l=1; l<=cynd; l++){
				PMT17O24->Fill(l, peaksNDMax[p][l][10]/peaksNDMax[p][l][3]);
			}

			c4.cd(1);
			PMT17O24->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/PMT17overPMT24MaxPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c4.SaveAs(nombrepdf.c_str());

			delete PMT17O24;
			PMTMax1=0;
			PMTMin1=0;


		}
	}
	if(nu!=0){
		for (int p=0; p<=nusec; p++){

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cynu; l++){
				if (NUPMTMax[p][l][1][1]!=0){
					if (PMTMax1==0) PMTMax1=NUPMTMax[p][l][1][1];
					if (NUPMTMax[p][l][1][1]>PMTMax1){
						PMTMax1=NUPMTMax[p][l][1][1];
					}
					if (PMTMin1==0) PMTMin1=NUPMTMax[p][l][1][1];
					if (PMTMin1>NUPMTMax[p][l][1][1]){
						PMTMin1=NUPMTMax[p][l][1][1];
					}
				}
				if (NUPMTMax[p][l][2][1]!=0){
					if (PMTMax2==0) PMTMax2=NUPMTMax[p][l][2][1];
					if (NUPMTMax[p][l][2][1]>PMTMax2){
						PMTMax2=NUPMTMax[p][l][2][1];
					}
					if (PMTMin2==0) PMTMin2=NUPMTMax[p][l][2][1];
					if (PMTMin2>NUPMTMax[p][l][2][1]){
						PMTMin2=NUPMTMax[p][l][2][1];
					}
				}
			}

			PMTMax1=PMTMax1 + motor9r[p];
			PMTMin1=PMTMin1 - motor9r[p];
			PMTMax2=PMTMax2 + motor10r[p];
			PMTMin2=PMTMin2 - motor10r[p];

			TH2* PMT25MaxMotor9 = new TH2F("PMT25 Max Motor9", "PMT25 Max peak Motor 9 change over time ", cynu, 0, cynu, cynu, PMTMin1, PMTMax1);
			PMT25MaxMotor9->SetStats(0);
			PMT25MaxMotor9->GetXaxis()->SetTitle("Nikko Up Peak Cycle (one cycle is 5 seconds)");
			PMT25MaxMotor9->GetYaxis()->SetTitle("PMT25 Motor 9 Max Peak position");

			TH2* PMT25MaxMotor10 = new TH2F("PMT25 Max Motor10", "PMT25 Max peak Motor 10 change over time ", cynu, 0, cynu, cynu, PMTMin2, PMTMax2);
			PMT25MaxMotor10->SetStats(0);
			PMT25MaxMotor10->GetXaxis()->SetTitle("Nikko Up Peak Cycle (one cycle is 5 seconds)");
			PMT25MaxMotor10->GetYaxis()->SetTitle("PMT25 Motor 10 Max Peak position");

			for(int l=1; l<=cynu; l++){
				PMT25MaxMotor9->Fill(l, NUPMTMax[p][l][1][1]);
				PMT25MaxMotor10->Fill(l, NUPMTMax[p][l][2][1]);
			}

			c3.cd(1);
			PMT25MaxMotor9->Draw("");
			c3.cd(2);
			PMT25MaxMotor10->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/PMT25MaxPeakPositionChangeRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c3.SaveAs(nombrepdf.c_str());

			delete PMT25MaxMotor9;
			delete PMT25MaxMotor10;
			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cynu; l++){
				if (peaksNUMax[p][l][1]!=0){
					if (PMTMax1==0) PMTMax1=peaksNUMax[p][l][1];
					if (peaksNUMax[p][l][1]>PMTMax1){
						PMTMax1=peaksNUMax[p][l][1];
					}
					if (PMTMin1==0) PMTMin1=peaksNUMax[p][l][1];
					if (PMTMin1>peaksNUMax[p][l][1]){
						PMTMin1=peaksNUMax[p][l][1];
					}
				}
				if (peaksNUMax[p][l][2]!=0){
					if (PMTMax2==0) PMTMax2=peaksNUMax[p][l][2];
					if (peaksNUMax[p][l][2]>PMTMax2){
						PMTMax2=peaksNUMax[p][l][2];
					}
					if (PMTMin2==0) PMTMin2=peaksNUMax[p][l][2];
					if (PMTMin2>peaksNUMax[p][l][2]){
						PMTMin2=peaksNUMax[p][l][2];
					}
				}
			}

			PMTMax1=PMTMax1 + motor9r[p];
			PMTMin1=PMTMin1 - motor9r[p];
			PMTMax2=PMTMax2 + motor10r[p];
			PMTMin2=PMTMin2 - motor10r[p];

			TH2* MaxMotor9 = new TH2F("Max Motor9", "Max peak Motor 9 change over time", cynu, 0, cynu, cynu, PMTMin1, PMTMax1);
			MaxMotor9->SetStats(0);
			MaxMotor9->GetXaxis()->SetTitle("Nikko Up Peak Cycle (one cycle is 5 secoNUs)");
			MaxMotor9->GetYaxis()->SetTitle("Motor 9 Max Peak position");

			TH2* MaxMotor10 = new TH2F("Max Motor10", "Max peak Motor 10 change over time", cynu, 0, cynu, cynu, PMTMin2, PMTMax2);
			MaxMotor10->SetStats(0);
			MaxMotor10->GetXaxis()->SetTitle("Nikko Up Peak Cycle (one cycle is 5 secoNUs)");
			MaxMotor10->GetYaxis()->SetTitle("Motor 10 Max Peak position");

			for(int l=1; l<=cynu; l++){
				MaxMotor9->Fill(l, peaksNUMax[p][l][1]);
				MaxMotor10->Fill(l, peaksNUMax[p][l][2]);
			}

			c3.cd(1);
			MaxMotor9->Draw("");
			c3.cd(2);
			MaxMotor10->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/Motor9Motor10MaxPeakPositionChangeRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c3.SaveAs(nombrepdf.c_str());

			delete MaxMotor9;
			delete MaxMotor10;
			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

			for(int l=1; l<=cynu; l++){
				if ((peaksNUMax[p][l][3]!=0) and (peaksNUMax[p][l][10]!=0)){
					if (PMTMax1==0) PMTMax1=peaksNUMax[p][l][10]/peaksNUMax[p][l][3];
					if ((peaksNUMax[p][l][10]/peaksNUMax[p][l][3])>PMTMax1){
						PMTMax1=peaksNUMax[p][l][10]/peaksNUMax[p][l][3];
					}
					if (PMTMin1==0) PMTMin1=peaksNUMax[p][l][10]/peaksNUMax[p][l][3];
					if (PMTMin1>(peaksNUMax[p][l][10]/peaksNUMax[p][l][3])){
						PMTMin1=peaksNUMax[p][l][10]/peaksNUMax[p][l][3];
					}
				}
			}

			TH2* PMT25O32 = new TH2F("PMT25 over PMT32", "PMT25 Over PMT32 of Max Peak ", cynu, 0, cynu, cynu, PMTMin1, PMTMax1);
			PMT25O32->SetStats(0);
			PMT25O32->GetXaxis()->SetTitle("Nikko Up Peak Cycle (one cycle is 5 seconds)");
			PMT25O32->GetYaxis()->SetTitle("PMT25/PMT32 of Max Peak");

			for(int l=1; l<=cynu; l++){
				PMT25O32->Fill(l, peaksNUMax[p][l][10]/peaksNUMax[p][l][3]);
			}

			c4.cd(1);
			PMT25O32->Draw("");

			section= to_string(p); 
			nombrepdf = string("PDF/PMT25overPMT32MaxPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c4.SaveAs(nombrepdf.c_str());

			delete PMT25O32;
			PMTMax1=0;
			PMTMin1=0;
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is to save the first and last (valid) peak cycle of each channel

	for (int p=0; p<=ousec; p++){ //For each area
		for(int l=1; l<=cyou; l++){ //And each peak cycle
			if (peaksOUMax[p][l][1] != 0){ //If the Max of that area and cycle is not zero
				LcyOU[p]=l; //We save the peak cycle number
			}
		}//Repeat until all peak cycles and the last non zero is saved
		for(int l=1; l<=cyou; l++){ //For each peak cycle
			if (peaksOUMax[p][l][1] != 0){//If the Max of that area and cycle is not zero
				FcyOU[p]=l;//We save the peak cycle number
				break; //and break to save the first non zero peak cycle
			}
		}
	}

	for (int p=0; p<=odsec; p++){ //Repeat for Oho Down
		for(int l=1; l<=cyod; l++){
			if (peaksODMax[p][l][1] != 0){
				LcyOD[p]=l;
			}
		}
		for(int l=1; l<=cyod; l++){
			if (peaksODMax[p][l][1] != 0){
				FcyOD[p]=l;
				break;
			}
		}
	}

	for (int p=0; p<=nusec; p++){ //Repeat for Nikko Up
		for(int l=1; l<=cynu; l++){
			if (peaksNUMax[p][l][1] != 0){
				LcyNU[p]=l;
			}
		}
		for(int l=1; l<=cynu; l++){
			if (peaksNUMax[p][l][1] != 0){
				FcyNU[p]=l;
				break;
			}
		}
	}

	for (int p=0; p<=ndsec; p++){ //Repeat for Nikko Down
		for(int l=1; l<=cynd; l++){
			if (peaksNDMax[p][l][1] != 0){
				LcyND[p]=l;
			}
		}
		for(int l=1; l<=cynd; l++){
			if (peaksNDMax[p][l][1] != 0){
				FcyND[p]=l;
				break;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Check if the peak moved between the first and the last cycle, and create the heat maps

	TCanvas c1;
	c1.Divide(2,2);

	TCanvas c2;
	c2.Divide(2,2);

	for (int j=0; j<=ousec; j++){ //For each section
		if((abs(peaksOUMax[j][FcyOU[j]][1]-peaksOUMax[j][LcyOU[j]][1])>1000)or(abs(peaksOUMax[j][FcyOU[j]][2]-peaksOUMax[j][LcyOU[j]][2])>1000)){ //Check the Max peak motor position for the first and last peak cycle, if the difference is bigger than 1000 for one of the motors

			for (int i=(OUsec[j]+1); i<=OUsec[j+1]; i++){
				if (peaksOU[i][1]!=0){
					if (PMTMax1==0) PMTMax1=peaksOU[i][1];
					if (peaksOU[i][1]>PMTMax1){
						PMTMax1=peaksOU[i][1];
					}
					if (PMTMin1==0) PMTMin1=peaksOU[i][1];
					if (PMTMin1>peaksOU[i][1]){
						PMTMin1=peaksOU[i][1];
					}
				}
				if (peaksOU[i][2]!=0){
					if (PMTMax2==0) PMTMax2=peaksOU[i][2];
					if (peaksOU[i][2]>PMTMax2){
						PMTMax2=peaksOU[i][2];
					}
					if (PMTMin2==0) PMTMin2=peaksOU[i][2];
					if (PMTMin2>peaksOU[i][2]){
						PMTMin2=peaksOU[i][2];
					}
				}
			}

			PMTMax1=PMTMax1 + motor1r[j];
			PMTMin1=PMTMin1 - motor1r[j];
			PMTMax2=PMTMax2 + motor2r[j];
			PMTMin2=PMTMin2 - motor2r[j];

			nbin1=(abs(PMTMax1-PMTMin1)/motor1r[j])+1; //Calculate the bin numbers
			nbin2=(abs(PMTMax2-PMTMin2)/motor2r[j])+1;

//Create the histograms for the first peak cycle

			TH2* PMT1HMF = new TH2F("PMT1", "First Entries Normalized PMT1", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT1HMF->SetStats(0);
			PMT1HMF->GetXaxis()->SetTitle("Motor 1 step");
			PMT1HMF->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT2HMF = new TH2F("PMT2", "First Entries Normalized PMT2", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT2HMF->SetStats(0);
			PMT2HMF->GetXaxis()->SetTitle("Motor 1 step");
			PMT2HMF->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT3HMF = new TH2F("PMT3", "First Entries Normalized PMT3", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT3HMF->SetStats(0);
			PMT3HMF->GetXaxis()->SetTitle("Motor 1 step");
			PMT3HMF->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT4HMF = new TH2F("PMT4", "First Entries Normalized PMT4", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT4HMF->SetStats(0);
			PMT4HMF->GetXaxis()->SetTitle("Motor 1 step");
			PMT4HMF->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT5HMF = new TH2F("PMT5", "First Entries Normalized PMT5", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT5HMF->SetStats(0);
			PMT5HMF->GetXaxis()->SetTitle("Motor 1 step");
			PMT5HMF->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT6HMF = new TH2F("PMT6", "First Entries Normalized PMT6", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT6HMF->SetStats(0);
			PMT6HMF->GetXaxis()->SetTitle("Motor 1 step");
			PMT6HMF->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT7HMF = new TH2F("PMT7", "First Entries Normalized PMT7", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT7HMF->SetStats(0);
			PMT7HMF->GetXaxis()->SetTitle("Motor 1 step");
			PMT7HMF->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT8HMF = new TH2F("PMT8", "First Entries Normalized PMT8", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT8HMF->SetStats(0);
			PMT8HMF->GetXaxis()->SetTitle("Motor 1 step");
			PMT8HMF->GetYaxis()->SetTitle("Motor 2 step");
			
			for (int i=(OUsec[j]+1); i<=OUsec[j+1]; i++){ //Fill the first peak cycle histograms
				PMT1HMF->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[FcyOU[j]][i][3]);
				PMT2HMF->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[FcyOU[j]][i][4]);
				PMT3HMF->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[FcyOU[j]][i][5]);
				PMT4HMF->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[FcyOU[j]][i][6]);
				PMT5HMF->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[FcyOU[j]][i][7]);
				PMT6HMF->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[FcyOU[j]][i][8]);
				PMT7HMF->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[FcyOU[j]][i][9]);
				PMT8HMF->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[FcyOU[j]][i][10]);
			}

//Make the heat maps for each pmt

			c1.cd(1);
			PMT1HMF->Draw("CONTZ");
			c1.cd(2);
			PMT2HMF->Draw("CONTZ");
			c1.cd(3);
			PMT3HMF->Draw("CONTZ");
			c1.cd(4);
			PMT4HMF->Draw("CONTZ");
			c2.cd(1);
			PMT5HMF->Draw("CONTZ");
			c2.cd(2);
			PMT6HMF->Draw("CONTZ");
			c2.cd(3);
			PMT7HMF->Draw("CONTZ");
			c2.cd(4);
			PMT8HMF->Draw("CONTZ");

			section= to_string(j); 
//Save the files
			nombrepdf = string("PDF/DataFirstEntrieHeatMapOUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf[");
			c1.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataFirstEntrieHeatMapOUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c1.SaveAs(nombrepdf.c_str());
			c2.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataFirstEntrieHeatMapOUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf]");
			c2.SaveAs(nombrepdf.c_str());

			delete PMT1HMF; //detele for memory
			delete PMT2HMF;
			delete PMT3HMF;
			delete PMT4HMF;
			delete PMT5HMF;
			delete PMT6HMF;
			delete PMT7HMF;
			delete PMT8HMF;

//Make last peak cycle histograms

			TH2* PMT1HML = new TH2F("PMT1", "Last Entries Normalized PMT1", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT1HML->SetStats(0);
			PMT1HML->GetXaxis()->SetTitle("Motor 1 step");
			PMT1HML->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT2HML = new TH2F("PMT2", "Last Entries Normalized PMT2", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT2HML->SetStats(0);
			PMT2HML->GetXaxis()->SetTitle("Motor 1 step");
			PMT2HML->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT3HML = new TH2F("PMT3", "Last Entries Normalized PMT3", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT3HML->SetStats(0);
			PMT3HML->GetXaxis()->SetTitle("Motor 1 step");
			PMT3HML->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT4HML = new TH2F("PMT4", "Last Entries Normalized PMT4", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT4HML->SetStats(0);
			PMT4HML->GetXaxis()->SetTitle("Motor 1 step");
			PMT4HML->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT5HML = new TH2F("PMT5", "Last Entries Normalized PMT5", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT5HML->SetStats(0);
			PMT5HML->GetXaxis()->SetTitle("Motor 1 step");
			PMT5HML->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT6HML = new TH2F("PMT6", "Last Entries Normalized PMT6", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT6HML->SetStats(0);
			PMT6HML->GetXaxis()->SetTitle("Motor 1 step");
			PMT6HML->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT7HML = new TH2F("PMT7", "Last Entries Normalized PMT7", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT7HML->SetStats(0);
			PMT7HML->GetXaxis()->SetTitle("Motor 1 step");
			PMT7HML->GetYaxis()->SetTitle("Motor 2 step");

			TH2* PMT8HML = new TH2F("PMT8", "Last Entries Normalized PMT8", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT8HML->SetStats(0);
			PMT8HML->GetXaxis()->SetTitle("Motor 1 step");
			PMT8HML->GetYaxis()->SetTitle("Motor 2 step");
			
			for (int i=(OUsec[j]+1); i<=OUsec[j+1]; i++){ //Fill the last peak cycle histograms
				PMT1HML->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[LcyOU[j]][i][3]);
				PMT2HML->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[LcyOU[j]][i][4]);
				PMT3HML->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[LcyOU[j]][i][5]);
				PMT4HML->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[LcyOU[j]][i][6]);
				PMT5HML->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[LcyOU[j]][i][7]);
				PMT6HML->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[LcyOU[j]][i][8]);
				PMT7HML->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[LcyOU[j]][i][9]);
				PMT8HML->Fill(peaksOU[i][1], peaksOU[i][2], peaksOUMatrix[LcyOU[j]][i][10]);
			}

//Make the heat maps for each pmt

			c1.cd(1);
			PMT1HML->Draw("CONTZ");
			c1.cd(2);
			PMT2HML->Draw("CONTZ");
			c1.cd(3);
			PMT3HML->Draw("CONTZ");
			c1.cd(4);
			PMT4HML->Draw("CONTZ");
			c2.cd(1);
			PMT5HML->Draw("CONTZ");
			c2.cd(2);
			PMT6HML->Draw("CONTZ");
			c2.cd(3);
			PMT7HML->Draw("CONTZ");
			c2.cd(4);
			PMT8HML->Draw("CONTZ");

//Save the files

			nombrepdf = string("PDF/DataLastEntrieHeatMapOUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf[");
			c1.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataLastEntrieHeatMapOUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c1.SaveAs(nombrepdf.c_str());
			c2.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataLastEntrieHeatMapOUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf]");
			c2.SaveAs(nombrepdf.c_str());

			delete PMT1HML; //detele for memory
			delete PMT2HML;
			delete PMT3HML;
			delete PMT4HML;
			delete PMT5HML;
			delete PMT6HML;
			delete PMT7HML;
			delete PMT8HML;

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;
		}
	}
	for (int j=0; j<=odsec; j++){ //For each section
		if((abs(peaksODMax[j][FcyOD[j]][1]-peaksODMax[j][LcyOD[j]][1])>1000)or(abs(peaksODMax[j][FcyOD[j]][2]-peaksODMax[j][LcyOD[j]][2])>1000)){ //Check the Max peak motor position for the first and last peak cycle, if the difference is bigger than 1000 for one of the motors

			for (int i=(ODsec[j]+1); i<=ODsec[j+1]; i++){
				if (peaksOD[i][1]!=0){
					if (PMTMax1==0) PMTMax1=peaksOD[i][1];
					if (peaksOD[i][1]>PMTMax1){
						PMTMax1=peaksOD[i][1];
					}
					if (PMTMin1==0) PMTMin1=peaksOD[i][1];
					if (PMTMin1>peaksOD[i][1]){
						PMTMin1=peaksOD[i][1];
					}
				}
				if (peaksOD[i][2]!=0){
					if (PMTMax2==0) PMTMax2=peaksOD[i][2];
					if (peaksOD[i][2]>PMTMax2){
						PMTMax2=peaksOD[i][2];
					}
					if (PMTMin2==0) PMTMin2=peaksOD[i][2];
					if (PMTMin2>peaksOD[i][2]){
						PMTMin2=peaksOD[i][2];
					}
				}
			}

			PMTMax1=PMTMax1 + motor3r[j];
			PMTMin1=PMTMin1 - motor3r[j];
			PMTMax2=PMTMax2 + motor4r[j];
			PMTMin2=PMTMin2 - motor4r[j];

			nbin1=(abs(PMTMax1-PMTMin1)/motor3r[j])+1; //Calculate the bin numbers
			nbin2=(abs(PMTMax2-PMTMin2)/motor4r[j])+1;

//Create the histograms for the first peak cycle

			TH2* PMT9HMF = new TH2F("PMT9", "First Entries Normalized PMT9", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT9HMF->SetStats(0);
			PMT9HMF->GetXaxis()->SetTitle("Motor 3 step");
			PMT9HMF->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT10HMF = new TH2F("PMT10", "First Entries Normalized PMT10", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT10HMF->SetStats(0);
			PMT10HMF->GetXaxis()->SetTitle("Motor 3 step");
			PMT10HMF->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT11HMF = new TH2F("PMT11", "First Entries Normalized PMT11", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT11HMF->SetStats(0);
			PMT11HMF->GetXaxis()->SetTitle("Motor 3 step");
			PMT11HMF->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT12HMF = new TH2F("PMT12", "First Entries Normalized PMT12", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT12HMF->SetStats(0);
			PMT12HMF->GetXaxis()->SetTitle("Motor 3 step");
			PMT12HMF->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT13HMF = new TH2F("PMT13", "First Entries Normalized PMT13", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT13HMF->SetStats(0);
			PMT13HMF->GetXaxis()->SetTitle("Motor 3 step");
			PMT13HMF->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT14HMF = new TH2F("PMT14", "First Entries Normalized PMT14", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT14HMF->SetStats(0);
			PMT14HMF->GetXaxis()->SetTitle("Motor 3 step");
			PMT14HMF->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT15HMF = new TH2F("PMT15", "First Entries Normalized PMT15", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT15HMF->SetStats(0);
			PMT15HMF->GetXaxis()->SetTitle("Motor 3 step");
			PMT15HMF->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT16HMF = new TH2F("PMT16", "First Entries Normalized PMT16", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT16HMF->SetStats(0);
			PMT16HMF->GetXaxis()->SetTitle("Motor 3 step");
			PMT16HMF->GetYaxis()->SetTitle("Motor 4 step");
			
			for (int i=(ODsec[j]+1); i<=ODsec[j+1]; i++){ //Fill the first peak cycle histograms
				PMT9HMF->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[FcyOD[j]][i][3]);
				PMT10HMF->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[FcyOD[j]][i][4]);
				PMT11HMF->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[FcyOD[j]][i][5]);
				PMT12HMF->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[FcyOD[j]][i][6]);
				PMT13HMF->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[FcyOD[j]][i][7]);
				PMT14HMF->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[FcyOD[j]][i][8]);
				PMT15HMF->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[FcyOD[j]][i][9]);
				PMT16HMF->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[FcyOD[j]][i][10]);
			}

//Make the heat maps for each pmt

			c1.cd(1);
			PMT9HMF->Draw("CONTZ");
			c1.cd(2);
			PMT10HMF->Draw("CONTZ");
			c1.cd(3);
			PMT11HMF->Draw("CONTZ");
			c1.cd(4);
			PMT12HMF->Draw("CONTZ");
			c2.cd(1);
			PMT13HMF->Draw("CONTZ");
			c2.cd(2);
			PMT14HMF->Draw("CONTZ");
			c2.cd(3);
			PMT15HMF->Draw("CONTZ");
			c2.cd(4);
			PMT16HMF->Draw("CONTZ");

			section= to_string(j); 
//Save the files
			nombrepdf = string("PDF/DataFirstEntrieHeatMapODPeakRun" + peaksnumberfile + "Sec" + section + ".pdf[");
			c1.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataFirstEntrieHeatMapODPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c1.SaveAs(nombrepdf.c_str());
			c2.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataFirstEntrieHeatMapODPeakRun" + peaksnumberfile + "Sec" + section + ".pdf]");
			c2.SaveAs(nombrepdf.c_str());

			delete PMT9HMF; //detele for memory
			delete PMT10HMF;
			delete PMT11HMF;
			delete PMT12HMF;
			delete PMT13HMF;
			delete PMT14HMF;
			delete PMT15HMF;
			delete PMT16HMF;

//Make last peak cycle histograms

			TH2* PMT9HML = new TH2F("PMT9", "Last Entries Normalized PMT9", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT9HML->SetStats(0);
			PMT9HML->GetXaxis()->SetTitle("Motor 3 step");
			PMT9HML->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT10HML = new TH2F("PMT10", "Last Entries Normalized PMT10", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT10HML->SetStats(0);
			PMT10HML->GetXaxis()->SetTitle("Motor 3 step");
			PMT10HML->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT11HML = new TH2F("PMT11", "Last Entries Normalized PMT11", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT11HML->SetStats(0);
			PMT11HML->GetXaxis()->SetTitle("Motor 3 step");
			PMT11HML->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT12HML = new TH2F("PMT12", "Last Entries Normalized PMT12", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT12HML->SetStats(0);
			PMT12HML->GetXaxis()->SetTitle("Motor 3 step");
			PMT12HML->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT13HML = new TH2F("PMT13", "Last Entries Normalized PMT13", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT13HML->SetStats(0);
			PMT13HML->GetXaxis()->SetTitle("Motor 3 step");
			PMT13HML->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT14HML = new TH2F("PMT14", "Last Entries Normalized PMT14", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT14HML->SetStats(0);
			PMT14HML->GetXaxis()->SetTitle("Motor 3 step");
			PMT14HML->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT15HML = new TH2F("PMT15", "Last Entries Normalized PMT15", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT15HML->SetStats(0);
			PMT15HML->GetXaxis()->SetTitle("Motor 3 step");
			PMT15HML->GetYaxis()->SetTitle("Motor 4 step");

			TH2* PMT16HML = new TH2F("PMT16", "Last Entries Normalized PMT16", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT16HML->SetStats(0);
			PMT16HML->GetXaxis()->SetTitle("Motor 3 step");
			PMT16HML->GetYaxis()->SetTitle("Motor 4 step");
			
			for (int i=(ODsec[j]+1); i<=ODsec[j+1]; i++){ //Fill the last peak cycle histograms
				PMT9HML->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[LcyOD[j]][i][3]);
				PMT10HML->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[LcyOD[j]][i][4]);
				PMT11HML->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[LcyOD[j]][i][5]);
				PMT12HML->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[LcyOD[j]][i][6]);
				PMT13HML->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[LcyOD[j]][i][7]);
				PMT14HML->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[LcyOD[j]][i][8]);
				PMT15HML->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[LcyOD[j]][i][9]);
				PMT16HML->Fill(peaksOD[i][1], peaksOD[i][2], peaksODMatrix[LcyOD[j]][i][10]);
			}

//Make the heat maps for each pmt

			c1.cd(1);
			PMT9HML->Draw("CONTZ");
			c1.cd(2);
			PMT10HML->Draw("CONTZ");
			c1.cd(3);
			PMT11HML->Draw("CONTZ");
			c1.cd(4);
			PMT12HML->Draw("CONTZ");
			c2.cd(1);
			PMT13HML->Draw("CONTZ");
			c2.cd(2);
			PMT14HML->Draw("CONTZ");
			c2.cd(3);
			PMT15HML->Draw("CONTZ");
			c2.cd(4);
			PMT16HML->Draw("CONTZ");

//Save the files

			nombrepdf = string("PDF/DataLastEntrieHeatMapODPeakRun" + peaksnumberfile + "Sec" + section + ".pdf[");
			c1.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataLastEntrieHeatMapODPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c1.SaveAs(nombrepdf.c_str());
			c2.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataLastEntrieHeatMapODPeakRun" + peaksnumberfile + "Sec" + section + ".pdf]");
			c2.SaveAs(nombrepdf.c_str());

			delete PMT9HML; //detele for memory
			delete PMT10HML;
			delete PMT11HML;
			delete PMT12HML;
			delete PMT13HML;
			delete PMT14HML;
			delete PMT15HML;
			delete PMT16HML;

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

		}
	}
	for (int j=0; j<=nusec; j++){ //For each section
		if((abs(peaksNUMax[j][FcyNU[j]][1]-peaksNUMax[j][LcyNU[j]][1])>1000)or(abs(peaksNUMax[j][FcyNU[j]][2]-peaksNUMax[j][LcyNU[j]][2])>1000)){ //Check the Max peak motor position for the first and last peak cycle, if the difference is bigger than 1000 for one of the motors

			for (int i=(NUsec[j]+1); i<=NUsec[j+1]; i++){
				if (peaksNU[i][1]!=0){
					if (PMTMax1==0) PMTMax1=peaksNU[i][1];
					if (peaksNU[i][1]>PMTMax1){
						PMTMax1=peaksNU[i][1];
					}
					if (PMTMin1==0) PMTMin1=peaksNU[i][1];
					if (PMTMin1>peaksNU[i][1]){
						PMTMin1=peaksNU[i][1];
					}
				}
				if (peaksNU[i][2]!=0){
					if (PMTMax2==0) PMTMax2=peaksNU[i][2];
					if (peaksNU[i][2]>PMTMax2){
						PMTMax2=peaksNU[i][2];
					}
					if (PMTMin2==0) PMTMin2=peaksNU[i][2];
					if (PMTMin2>peaksNU[i][2]){
						PMTMin2=peaksNU[i][2];
					}
				}
			}

			PMTMax1=PMTMax1 + motor9r[j];
			PMTMin1=PMTMin1 - motor9r[j];
			PMTMax2=PMTMax2 + motor10r[j];
			PMTMin2=PMTMin2 - motor10r[j];

			nbin1=(abs(PMTMax1-PMTMin1)/motor9r[j])+1; //Calculate the bin numbers
			nbin2=(abs(PMTMax2-PMTMin2)/motor10r[j])+1;

//Create the histograms for the first peak cycle

			TH2* PMT25HMF = new TH2F("PMT25", "First Entries Normalized PMT25", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT25HMF->SetStats(0);
			PMT25HMF->GetXaxis()->SetTitle("Motor 9 step");
			PMT25HMF->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT26HMF = new TH2F("PMT26", "First Entries Normalized PMT26", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT26HMF->SetStats(0);
			PMT26HMF->GetXaxis()->SetTitle("Motor 9 step");
			PMT26HMF->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT27HMF = new TH2F("PMT27", "First Entries Normalized PMT27", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT27HMF->SetStats(0);
			PMT27HMF->GetXaxis()->SetTitle("Motor 9 step");
			PMT27HMF->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT28HMF = new TH2F("PMT28", "First Entries Normalized PMT28", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT28HMF->SetStats(0);
			PMT28HMF->GetXaxis()->SetTitle("Motor 9 step");
			PMT28HMF->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT29HMF = new TH2F("PMT29", "First Entries Normalized PMT29", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT29HMF->SetStats(0);
			PMT29HMF->GetXaxis()->SetTitle("Motor 9 step");
			PMT29HMF->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT30HMF = new TH2F("PMT30", "First Entries Normalized PMT30", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT30HMF->SetStats(0);
			PMT30HMF->GetXaxis()->SetTitle("Motor 9 step");
			PMT30HMF->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT31HMF = new TH2F("PMT31", "First Entries Normalized PMT31", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT31HMF->SetStats(0);
			PMT31HMF->GetXaxis()->SetTitle("Motor 9 step");
			PMT31HMF->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT32HMF = new TH2F("PMT32", "First Entries Normalized PMT32", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT32HMF->SetStats(0);
			PMT32HMF->GetXaxis()->SetTitle("Motor 9 step");
			PMT32HMF->GetYaxis()->SetTitle("Motor 10 step");
			
			for (int i=(NUsec[j]+1); i<=NUsec[j+1]; i++){ //Fill the first peak cycle histograms
				PMT25HMF->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[FcyNU[j]][i][3]);
				PMT26HMF->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[FcyNU[j]][i][4]);
				PMT27HMF->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[FcyNU[j]][i][5]);
				PMT28HMF->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[FcyNU[j]][i][6]);
				PMT29HMF->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[FcyNU[j]][i][7]);
				PMT30HMF->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[FcyNU[j]][i][8]);
				PMT31HMF->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[FcyNU[j]][i][9]);
				PMT32HMF->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[FcyNU[j]][i][10]);
			}

//Make the heat maps for each pmt

			c1.cd(1);
			PMT25HMF->Draw("CONTZ");
			c1.cd(2);
			PMT26HMF->Draw("CONTZ");
			c1.cd(3);
			PMT27HMF->Draw("CONTZ");
			c1.cd(4);
			PMT28HMF->Draw("CONTZ");
			c2.cd(1);
			PMT29HMF->Draw("CONTZ");
			c2.cd(2);
			PMT30HMF->Draw("CONTZ");
			c2.cd(3);
			PMT31HMF->Draw("CONTZ");
			c2.cd(4);
			PMT32HMF->Draw("CONTZ");

			section= to_string(j); 
//Save the files
			nombrepdf = string("PDF/DataFirstEntrieHeatMapNUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf[");
			c1.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataFirstEntrieHeatMapNUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c1.SaveAs(nombrepdf.c_str());
			c2.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataFirstEntrieHeatMapNUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf]");
			c2.SaveAs(nombrepdf.c_str());

			delete PMT25HMF; //detele for memory
			delete PMT26HMF;
			delete PMT27HMF;
			delete PMT28HMF;
			delete PMT29HMF;
			delete PMT30HMF;
			delete PMT31HMF;
			delete PMT32HMF;

//Make last peak cycle histograms

			TH2* PMT25HML = new TH2F("PMT25", "Last Entries Normalized PMT25", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT25HML->SetStats(0);
			PMT25HML->GetXaxis()->SetTitle("Motor 9 step");
			PMT25HML->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT26HML = new TH2F("PMT26", "Last Entries Normalized PMT26", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT26HML->SetStats(0);
			PMT26HML->GetXaxis()->SetTitle("Motor 9 step");
			PMT26HML->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT27HML = new TH2F("PMT27", "Last Entries Normalized PMT27", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT27HML->SetStats(0);
			PMT27HML->GetXaxis()->SetTitle("Motor 9 step");
			PMT27HML->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT28HML = new TH2F("PMT28", "Last Entries Normalized PMT28", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT28HML->SetStats(0);
			PMT28HML->GetXaxis()->SetTitle("Motor 9 step");
			PMT28HML->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT29HML = new TH2F("PMT29", "Last Entries Normalized PMT29", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT29HML->SetStats(0);
			PMT29HML->GetXaxis()->SetTitle("Motor 9 step");
			PMT29HML->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT30HML = new TH2F("PMT30", "Last Entries Normalized PMT30", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT30HML->SetStats(0);
			PMT30HML->GetXaxis()->SetTitle("Motor 9 step");
			PMT30HML->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT31HML = new TH2F("PMT31", "Last Entries Normalized PMT31", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT31HML->SetStats(0);
			PMT31HML->GetXaxis()->SetTitle("Motor 9 step");
			PMT31HML->GetYaxis()->SetTitle("Motor 10 step");

			TH2* PMT32HML = new TH2F("PMT32", "Last Entries Normalized PMT32", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT32HML->SetStats(0);
			PMT32HML->GetXaxis()->SetTitle("Motor 9 step");
			PMT32HML->GetYaxis()->SetTitle("Motor 10 step");
			
			for (int i=(NUsec[j]+1); i<=NUsec[j+1]; i++){ //Fill the last peak cycle histograms
				PMT25HML->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[LcyNU[j]][i][3]);
				PMT26HML->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[LcyNU[j]][i][4]);
				PMT27HML->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[LcyNU[j]][i][5]);
				PMT28HML->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[LcyNU[j]][i][6]);
				PMT29HML->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[LcyNU[j]][i][7]);
				PMT30HML->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[LcyNU[j]][i][8]);
				PMT31HML->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[LcyNU[j]][i][9]);
				PMT32HML->Fill(peaksNU[i][1], peaksNU[i][2], peaksNUMatrix[LcyNU[j]][i][10]);
			}

//Make the heat maps for each pmt

			c1.cd(1);
			PMT25HML->Draw("CONTZ");
			c1.cd(2);
			PMT26HML->Draw("CONTZ");
			c1.cd(3);
			PMT27HML->Draw("CONTZ");
			c1.cd(4);
			PMT28HML->Draw("CONTZ");
			c2.cd(1);
			PMT29HML->Draw("CONTZ");
			c2.cd(2);
			PMT30HML->Draw("CONTZ");
			c2.cd(3);
			PMT31HML->Draw("CONTZ");
			c2.cd(4);
			PMT32HML->Draw("CONTZ");

//Save the files

			nombrepdf = string("PDF/DataLastEntrieHeatMapNUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf[");
			c1.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataLastEntrieHeatMapNUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c1.SaveAs(nombrepdf.c_str());
			c2.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataLastEntrieHeatMapNUPeakRun" + peaksnumberfile + "Sec" + section + ".pdf]");
			c2.SaveAs(nombrepdf.c_str());

			delete PMT25HML; //detele for memory
			delete PMT26HML;
			delete PMT27HML;
			delete PMT28HML;
			delete PMT29HML;
			delete PMT30HML;
			delete PMT31HML;
			delete PMT32HML;

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;

		}
	}
	for (int j=0; j<=ndsec; j++){ //For each section
		if((abs(peaksNDMax[j][FcyND[j]][1]-peaksNDMax[j][LcyND[j]][1])>1000)or(abs(peaksNDMax[j][FcyND[j]][2]-peaksNDMax[j][LcyND[j]][2])>1000)){ //Check the Max peak motor position for the first and last peak cycle, if the difference is bigger than 1000 for one of the motors

			for (int i=(NDsec[j]+1); i<=NDsec[j+1]; i++){
				if (peaksND[i][1]!=0){
					if (PMTMax1==0) PMTMax1=peaksND[i][1];
					if (peaksND[i][1]>PMTMax1){
						PMTMax1=peaksND[i][1];
					}
					if (PMTMin1==0) PMTMin1=peaksND[i][1];
					if (PMTMin1>peaksND[i][1]){
						PMTMin1=peaksND[i][1];
					}
				}
				if (peaksND[i][2]!=0){
					if (PMTMax2==0) PMTMax2=peaksND[i][2];
					if (peaksND[i][2]>PMTMax2){
						PMTMax2=peaksND[i][2];
					}
					if (PMTMin2==0) PMTMin2=peaksND[i][2];
					if (PMTMin2>peaksND[i][2]){
						PMTMin2=peaksND[i][2];
					}
				}
			}

			PMTMax1=PMTMax1 + motor11r[j];
			PMTMin1=PMTMin1 - motor11r[j];
			PMTMax2=PMTMax2 + motor12r[j];
			PMTMin2=PMTMin2 - motor12r[j];

			nbin1=(abs(PMTMax1-PMTMin1)/motor11r[j])+1; //Calculate the bin NDmbers
			nbin2=(abs(PMTMax2-PMTMin2)/motor12r[j])+1;

//Create the histograms for the first peak cycle

			TH2* PMT17HMF = new TH2F("PMT17", "First Entries Normalized PMT17", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT17HMF->SetStats(0);
			PMT17HMF->GetXaxis()->SetTitle("Motor 11 step");
			PMT17HMF->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT18HMF = new TH2F("PMT18", "First Entries Normalized PMT18", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT18HMF->SetStats(0);
			PMT18HMF->GetXaxis()->SetTitle("Motor 11 step");
			PMT18HMF->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT19HMF = new TH2F("PMT19", "First Entries Normalized PMT19", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT19HMF->SetStats(0);
			PMT19HMF->GetXaxis()->SetTitle("Motor 11 step");
			PMT19HMF->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT20HMF = new TH2F("PMT20", "First Entries Normalized PMT20", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT20HMF->SetStats(0);
			PMT20HMF->GetXaxis()->SetTitle("Motor 11 step");
			PMT20HMF->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT21HMF = new TH2F("PMT21", "First Entries Normalized PMT21", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT21HMF->SetStats(0);
			PMT21HMF->GetXaxis()->SetTitle("Motor 11 step");
			PMT21HMF->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT22HMF = new TH2F("PMT22", "First Entries Normalized PMT22", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT22HMF->SetStats(0);
			PMT22HMF->GetXaxis()->SetTitle("Motor 11 step");
			PMT22HMF->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT23HMF = new TH2F("PMT23", "First Entries Normalized PMT23", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT23HMF->SetStats(0);
			PMT23HMF->GetXaxis()->SetTitle("Motor 11 step");
			PMT23HMF->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT24HMF = new TH2F("PMT24", "First Entries Normalized PMT24", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT24HMF->SetStats(0);
			PMT24HMF->GetXaxis()->SetTitle("Motor 11 step");
			PMT24HMF->GetYaxis()->SetTitle("Motor 12 step");
			
			for (int i=(NDsec[j]+1); i<=NDsec[j+1]; i++){ //Fill the first peak cycle histograms
				PMT17HMF->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[FcyND[j]][i][3]);
				PMT18HMF->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[FcyND[j]][i][4]);
				PMT19HMF->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[FcyND[j]][i][5]);
				PMT20HMF->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[FcyND[j]][i][6]);
				PMT21HMF->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[FcyND[j]][i][7]);
				PMT22HMF->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[FcyND[j]][i][8]);
				PMT23HMF->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[FcyND[j]][i][9]);
				PMT24HMF->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[FcyND[j]][i][10]);
			}

//Make the heat maps for each pmt

			c1.cd(1);
			PMT17HMF->Draw("CONTZ");
			c1.cd(2);
			PMT18HMF->Draw("CONTZ");
			c1.cd(3);
			PMT19HMF->Draw("CONTZ");
			c1.cd(4);
			PMT20HMF->Draw("CONTZ");
			c2.cd(1);
			PMT21HMF->Draw("CONTZ");
			c2.cd(2);
			PMT22HMF->Draw("CONTZ");
			c2.cd(3);
			PMT23HMF->Draw("CONTZ");
			c2.cd(4);
			PMT24HMF->Draw("CONTZ");

			section= to_string(j); 
//Save the files
			nombrepdf = string("PDF/DataFirstEntrieHeatMapNDPeakRun" + peaksnumberfile + "Sec" + section + ".pdf[");
			c1.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataFirstEntrieHeatMapNDPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c1.SaveAs(nombrepdf.c_str());
			c2.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataFirstEntrieHeatMapNDPeakRun" + peaksnumberfile + "Sec" + section + ".pdf]");
			c2.SaveAs(nombrepdf.c_str());

			delete PMT17HMF; //detele for memory
			delete PMT18HMF;
			delete PMT19HMF;
			delete PMT20HMF;
			delete PMT21HMF;
			delete PMT22HMF;
			delete PMT23HMF;
			delete PMT24HMF;

//Make last peak cycle histograms

			TH2* PMT17HML = new TH2F("PMT17", "Last Entries Normalized PMT17", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT17HML->SetStats(0);
			PMT17HML->GetXaxis()->SetTitle("Motor 11 step");
			PMT17HML->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT18HML = new TH2F("PMT18", "Last Entries Normalized PMT18", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT18HML->SetStats(0);
			PMT18HML->GetXaxis()->SetTitle("Motor 11 step");
			PMT18HML->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT19HML = new TH2F("PMT19", "Last Entries Normalized PMT19", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT19HML->SetStats(0);
			PMT19HML->GetXaxis()->SetTitle("Motor 11 step");
			PMT19HML->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT20HML = new TH2F("PMT20", "Last Entries Normalized PMT20", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT20HML->SetStats(0);
			PMT20HML->GetXaxis()->SetTitle("Motor 11 step");
			PMT20HML->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT21HML = new TH2F("PMT21", "Last Entries Normalized PMT21", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT21HML->SetStats(0);
			PMT21HML->GetXaxis()->SetTitle("Motor 11 step");
			PMT21HML->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT22HML = new TH2F("PMT22", "Last Entries Normalized PMT22", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT22HML->SetStats(0);
			PMT22HML->GetXaxis()->SetTitle("Motor 11 step");
			PMT22HML->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT23HML = new TH2F("PMT23", "Last Entries Normalized PMT23", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT23HML->SetStats(0);
			PMT23HML->GetXaxis()->SetTitle("Motor 11 step");
			PMT23HML->GetYaxis()->SetTitle("Motor 12 step");

			TH2* PMT24HML = new TH2F("PMT24", "Last Entries Normalized PMT24", nbin1, PMTMin1, PMTMax1, nbin2, PMTMin2, PMTMax2);
			PMT24HML->SetStats(0);
			PMT24HML->GetXaxis()->SetTitle("Motor 11 step");
			PMT24HML->GetYaxis()->SetTitle("Motor 12 step");
			
			for (int i=(NDsec[j]+1); i<=NDsec[j+1]; i++){ //Fill the last peak cycle histograms
				PMT17HML->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[LcyND[j]][i][3]);
				PMT18HML->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[LcyND[j]][i][4]);
				PMT19HML->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[LcyND[j]][i][5]);
				PMT20HML->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[LcyND[j]][i][6]);
				PMT21HML->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[LcyND[j]][i][7]);
				PMT22HML->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[LcyND[j]][i][8]);
				PMT23HML->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[LcyND[j]][i][9]);
				PMT24HML->Fill(peaksND[i][1], peaksND[i][2], peaksNDMatrix[LcyND[j]][i][10]);
			}

//Make the heat maps for each pmt

			c1.cd(1);
			PMT17HML->Draw("CONTZ");
			c1.cd(2);
			PMT18HML->Draw("CONTZ");
			c1.cd(3);
			PMT19HML->Draw("CONTZ");
			c1.cd(4);
			PMT20HML->Draw("CONTZ");
			c2.cd(1);
			PMT21HML->Draw("CONTZ");
			c2.cd(2);
			PMT22HML->Draw("CONTZ");
			c2.cd(3);
			PMT23HML->Draw("CONTZ");
			c2.cd(4);
			PMT24HML->Draw("CONTZ");

//Save the files

			nombrepdf = string("PDF/DataLastEntrieHeatMapNDPeakRun" + peaksnumberfile + "Sec" + section + ".pdf[");
			c1.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataLastEntrieHeatMapNDPeakRun" + peaksnumberfile + "Sec" + section + ".pdf");
			c1.SaveAs(nombrepdf.c_str());
			c2.SaveAs(nombrepdf.c_str());
			nombrepdf = string("PDF/DataLastEntrieHeatMapNDPeakRun" + peaksnumberfile + "Sec" + section + ".pdf]");
			c2.SaveAs(nombrepdf.c_str());

			delete PMT17HML; //detele for memory
			delete PMT18HML;
			delete PMT19HML;
			delete PMT20HML;
			delete PMT21HML;
			delete PMT22HML;
			delete PMT23HML;
			delete PMT24HML;

			PMTMax1=0;
			PMTMin1=0;
			PMTMax2=0;
			PMTMin2=0;
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Create data quality graphs, specifically for PMT1 on the OU optical channel

	TCanvas c5;
	c5.Divide(2,2);

	TCanvas c6;
	c6.Divide(2,2);

	for (int p=0; p<=ousec; p++){
		for(int l=1; l<=cyou; l++){
			for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){
				for(int j=0; j<8; j++){
					m0[p][l][j+1] += peaksOUMatrix[l][i][j+3];
				}
			}
			for (int i=(OUsec[p]+1); i<=OUsec[p+1]; i++){
				for(int j=0; j<8; j++){
				Motxave[p][l][j+1]+=(peaksOUMatrix[l][i][j+3]*peaksOUMatrix[l][i][1])/m0[p][l][j+1];
				Motyave[p][l][j+1]+=(peaksOUMatrix[l][i][j+3]*peaksOUMatrix[l][i][2])/m0[p][l][j+1];
				DMotxave[p][l][j+1]+=(peaksOUMatrix[l][i][j+3]*peaksOUMatrix[l][i][1]*peaksOUMatrix[l][i][1])/m0[p][l][j+1];
				DMotyave[p][l][j+1]+=(peaksOUMatrix[l][i][j+3]*peaksOUMatrix[l][i][2]*peaksOUMatrix[l][i][2])/m0[p][l][j+1];
				AMotxMoty[p][l][j+1]+=(peaksOUMatrix[l][i][j+3]*peaksOUMatrix[l][i][1]*peaksOUMatrix[l][i][2])/m0[p][l][j+1];
				}
			}
		}
		for(int l=1; l<=cyou; l++){
			for(int j=0; j<8; j++){
				if (m0[p][l][j+1]>m0max[j+1]){
					m0max[j+1]=m0[p][l][j+1];
				}
				if (m0min[j+1]==0) m0min[j+1]=m0[p][l][j+1];
				if (m0[p][l][j+1]<m0min[j+1]){
					m0min[j+1]=m0[p][l][j+1];
				}
				if(Motxave[p][l][j+1]!=0){
					if (Motxave[p][l][j+1]>Motxavemax[j+1]){
						Motxavemax[j+1]=Motxave[p][l][j+1];
					}
					if (Motxavemin[j+1]==0) Motxavemin[j+1]=Motxave[p][l][j+1];
					if (Motxave[p][l][j+1]<Motxavemin[j+1]){
						Motxavemin[j+1]=Motxave[p][l][j+1];
					}
				}
				if(Motyave[p][l][j+1]!=0){
					if (Motyave[p][l][j+1]>Motyavemax[j+1]){
						Motyavemax[j+1]=Motyave[p][l][j+1];
					}
					if (Motyavemin[j+1]==0) Motyavemin[j+1]=Motyave[p][l][j+1];
					if (Motyave[p][l][j+1]<Motyavemin[j+1]){
						Motyavemin[j+1]=Motyave[p][l][j+1];
					}
				}
				if(DMotxave[p][l][j+1]!=0){
					if (DMotxave[p][l][j+1]>DMotxavemax[j+1]){
						DMotxavemax[j+1]=DMotxave[p][l][j+1];
					}
					if (DMotxavemin[j+1]==0) DMotxavemin[j+1]=DMotxave[p][l][j+1];
					if (DMotxave[p][l][j+1]<DMotxavemin[j+1]){
						DMotxavemin[j+1]=DMotxave[p][l][j+1];
					}
				}
				if(DMotyave[p][l][j+1]!=0){
					if (DMotyave[p][l][j+1]>DMotyavemax[j+1]){
						DMotyavemax[j+1]=DMotyave[p][l][j+1];
					}
					if (DMotyavemin[j+1]==0) DMotyavemin[j+1]=DMotyave[p][l][j+1];
					if (DMotyave[p][l][j+1]<DMotyavemin[j+1]){
						DMotyavemin[j+1]=DMotyave[p][l][j+1];
					}
				}
				if(AMotxMoty[p][l][j+1]!=0){
					if (AMotxMoty[p][l][j+1]>AMotxMotymax[j+1]){
						AMotxMotymax[j+1]=AMotxMoty[p][l][j+1];
					}
					if (AMotxMotymin[j+1]==0) AMotxMotymin[j+1]=AMotxMoty[p][l][j+1];
					if (AMotxMoty[p][l][j+1]<AMotxMotymin[j+1]){
						AMotxMotymin[j+1]=AMotxMoty[p][l][j+1];
					}
				}
			}
		}

		TH2* m0OU1 = new TH2F("m0 for PMT1", "m0 for PMT1 change over time ", cyou, 0, cyou, cyou, m0min[1], m0max[1]);
		m0OU1->SetStats(0);
		m0OU1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
		m0OU1->GetYaxis()->SetTitle("PMT1 m0 rate");

		TH2* Motor1Ave1 = new TH2F("PMT1 Motor1 Average", "Motor1 average for PMT1 change over time ", cyou, 0, cyou, cyou, Motxavemax[1], Motxavemax[1]);
		Motor1Ave1->SetStats(0);
		Motor1Ave1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
		Motor1Ave1->GetYaxis()->SetTitle("PMT1 Motor1 Ave rate");

		TH2* Motor2Ave1 = new TH2F("PMT1 Motor2 Average", "Motor2 average for PMT1 change over time ", cyou, 0, cyou, cyou, Motyavemin[1], Motyavemax[1]);
		Motor2Ave1->SetStats(0);
		Motor2Ave1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
		Motor2Ave1->GetYaxis()->SetTitle("PMT1 Motor2 Ave rate");

		TH2* DMotor1Ave1 = new TH2F("PMT1 DMotor1 Average", "DMotor1 average for PMT1 change over time ", cyou, 0, cyou, cyou, DMotxavemin[1], DMotxavemax[1]);
		DMotor1Ave1->SetStats(0);
		DMotor1Ave1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
		DMotor1Ave1->GetYaxis()->SetTitle("PMT1 DMotor1 Ave rate");

		TH2* DMotor2Ave1 = new TH2F("PMT1 DMotor2 Average", "DMotor2 average for PMT1 change over time ", cyou, 0, cyou, cyou, DMotyavemin[1], DMotyavemax[1]);
		DMotor2Ave1->SetStats(0);
		DMotor2Ave1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
		DMotor2Ave1->GetYaxis()->SetTitle("PMT1 DMotor2 Ave rate");

		TH2* AMotor1Motor2Ave1 = new TH2F("PMT1 A Motor1 Motor2", "A Motor1 Motor2 average for PMT1 change over time ", cyou, 0, cyou, cyou, AMotxMotymin[1], AMotxMotymax[1]);
		AMotor1Motor2Ave1->SetStats(0);
		AMotor1Motor2Ave1->GetXaxis()->SetTitle("Oho Up Peak Cycle (one cycle is 5 seconds)");
		AMotor1Motor2Ave1->GetYaxis()->SetTitle("PMT1 A Motor1 Motor2 Ave rate");

		for(int l=1; l<=cyou; l++){
			m0OU1->Fill(l, m0[p][l][1]);
			Motor1Ave1->Fill(l, Motxave[p][l][1]);
			Motor2Ave1->Fill(l, Motyave[p][l][1]);
			DMotor1Ave1->Fill(l, DMotxave[p][l][1]);
			DMotor2Ave1->Fill(l, DMotyave[p][l][1]);
			AMotor1Motor2Ave1->Fill(l, AMotxMoty[p][l][1]);
		}

		c5.cd(1);
		m0OU1->Draw("");
		c5.cd(2);
		Motor1Ave1->Draw("");
		c5.cd(3);
		Motor2Ave1->Draw("");

		c6.cd(1);
		AMotor1Motor2Ave1->Draw("");
		c6.cd(2);
		DMotor1Ave1->Draw("");
		c6.cd(3);
		DMotor2Ave1->Draw("");

		section= to_string(p);

		nombrepdf = string("PDF/AverageMotor1Motor2PMT1Run" + peaksnumberfile + "Sec" + section + ".pdf[");
		c5.SaveAs(nombrepdf.c_str());
		nombrepdf = string("PDF/AverageMotor1Motor2PMT1Run" + peaksnumberfile + "Sec" + section + ".pdf");
		c5.SaveAs(nombrepdf.c_str());
		c6.SaveAs(nombrepdf.c_str());
		nombrepdf = string("PDF/AverageMotor1Motor2PMT1Run" + peaksnumberfile + "Sec" + section + ".pdf]");
		c6.SaveAs(nombrepdf.c_str());

		delete m0OU1;
		delete Motor1Ave1;
		delete Motor2Ave1;
		delete DMotor1Ave1;
		delete DMotor2Ave1;
		delete AMotor1Motor2Ave1;

		for(int j=0; j<8; j++){
			m0max[j+1]=0;
			m0min[j+1]=0;
			Motxavemax[j+1]=0;
			Motxavemin[j+1]=0;
			Motyavemax[j+1]=0;
			Motyavemin[j+1]=0;
			DMotxavemax[j+1]=0;
			DMotxavemin[j+1]=0;
			DMotyavemax[j+1]=0;
			DMotyavemin[j+1]=0;
			AMotxMotymax[j+1]=0;
			AMotxMotymin[j+1]=0;
		}
	}
	for (int p=0; p<=ousec; p++){
		for(int l=1; l<=cyou; l++){
			for(int j=0; j<8; j++){
				m0[p][l][j+1]=0;
				Motxave[p][l][j+1]=0;
				Motyave[p][l][j+1]=0;
				DMotxave[p][l][j+1]=0;
				DMotyave[p][l][j+1]=0;
				AMotxMoty[p][l][j+1]=0;
			}
		}
	}
return(0);
}

