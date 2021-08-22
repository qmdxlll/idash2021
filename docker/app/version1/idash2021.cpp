#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>
#include <random>
#include <math.h>
#include <cmath>
#include <algorithm>
#include <time.h>
#include "tfhe.h"
#include "tfhe_io.h"

using namespace std;

// Security constants
#define SECALPHA pow(2., -20)
const int d_input = 7500;
const int d_fc0 = 256;
const int d_output = 4;
const double alpha = SECALPHA;
const int spc_msg = 3000;   //ciphertext space
//128bits
const int32_t N = 1024;
const int32_t k = 1;
const double alpha_min = pow(2.,-25);//128bit
const double alpha_max = 0.012467;//128bit
//TLWE
TLweParams *tlweparams = new_TLweParams(N, k, alpha_min, alpha_max); //les deux alpha mis un peu au hasard
TLweKey *tlwekey = new_TLweKey(tlweparams);
TorusPolynomial *dechif = new_TorusPolynomial_array(d_output*2,N);
const Torus32 mu_total = modSwitchToTorus32(1, spc_msg);
//float
Torus32 floatmodSwitchToTorus32(float mu, int32_t Msize){
    uint64_t interv = ((UINT64_C(1)<<63)/Msize)*2; // width of each intervall
    uint64_t phase64 = mu*interv;
    //floor to the nearest multiples of interv
    return phase64>>32;
}
////fc
TLweSample *fully_connection(TLweSample *input, vector<vector<float>> weight,int number)
{
	TLweSample *result = new_TLweSample_array(2*number, tlweparams);//16
	int len1 = weight[0].size(), len2 = weight.size();//7500,1024
	for (int i = 0; i < len2; i++)//1024
	{
	    tLweClear(result+i,tlweparams);
	    tLweClear(result+i+number,tlweparams);
		for (int j = 0; j < len1; j++)//7500
        {
            int wt=weight[i][j];
            tLweAddMulTo(result+i,wt,input+j,tlweparams);//pres=pres+wt*input
            tLweAddMulTo(result+i+number,wt,input+j+len1,tlweparams);//pres=pres+wt*input
        }
    }
    return result;
}
void readweight0(ifstream &ifile, vector<vector<float>> &weight)  //读取权重
{
	string str;
	while (getline(ifile, str))
	{
		istringstream str1(str);
		vector<float> temp;
		float aa;
		while (str1 >> aa)
			temp.push_back(round(aa));
		weight.push_back(temp);
	}
}
void readweight1(ifstream &ifile, vector<vector<float>> &weight)  //读取权重
{
	string str;
	while (getline(ifile, str))
	{
		istringstream str1(str);
		vector<float> temp;
		float aa;
		while (str1 >> aa)
			temp.push_back(round(aa/10));
		weight.push_back(temp);
	}
}
void readingdata(ifstream &file, vector<vector<int>> &input)//读取输入数据
{
	string str;
	vector<vector<int>> temp;
	//getline 读取一行数据，遇到换行结束
    while (getline(file, str))    //将数据全部读取入sampleid数组
	{
        vector<int> temp2;
        istringstream str1(str);
        string ss;
        for (int i = 0; i < d_input; i++) {
            str1 >> ss;
            temp2.push_back(atoi(ss.c_str()));
        }
        temp.push_back(temp2);
	}
	file.close();
	for (int i = 0; i < 2000; i++)
	{
		input.push_back(temp[i]);
	}
}
void EncTLWE(TLweSample *cipher,vector<vector<int>> input)
{
    int len1 = input[0].size(), len2 = input.size();//256,2000
    //MESSAGE
    TorusPolynomial *mu = new_TorusPolynomial(N);
    for(int k=0;k<2;k++){
        for (int32_t i = 0; i < len1; ++i) {//256
            for(int32_t j = 0; j < len2/2; ++j){//1000
                float plain = (float)input[j+k*len2/2][i]/1000;
                mu->coefsT[j] = floatmodSwitchToTorus32(plain, spc_msg);
            }
            tLweSymEncrypt(cipher+i+k*len1, mu, alpha, tlwekey); // ENCRYPTION
        }
    }
}
vector<int> Pre(TorusPolynomial *cipher,int num)
{
       float max_score = -100000;
       vector<int> pre = {0,0,0,0,-1};
       for(int ix=0; ix<d_output; ix++)
       {
          float score = (cipher+ix)->coefsT[num]/mu_total;
          pre[ix]=score;
          if(score > max_score)
          {
              max_score = score;
              pre[4] = ix;
          }
       }
       return pre;
}
string Prints(int prediction)
{
    string sss;
    if(prediction == 0){
        sss= "B.1.427";
    }
    else if(prediction ==1){
        sss= "B.1.1.7";
    }
    else if(prediction ==2){
        sss= "P.1";
    }
    else{
        sss="B.1.526";
    }
    return sss;
}
int main()
{
    cout << "Hello, IDASH2021!" << endl;
    clock_t startread,endread;
    startread=clock();
	vector<vector<int>> input;  //training data
   	 ifstream ifile("test.txt");
	readingdata(ifile, input);
    	ifile.close();
	cout<<"input size is "<<input.size()<<"*"<<input[0].size()<<endl;
	vector<vector<float>> fc0w;
	vector<vector<float>> fc1w;
	ifstream fc0("weight/fc0w.txt");
	readweight0(fc0, fc0w);//1024*7500
	fc0.close();
    	ifstream fc1("weight/fc1w.txt");
	readweight1(fc1, fc1w);//4*1024
	fc1.close();
    endread =clock();
    cout<<"read data time is:"<<(double)(endread-startread)/CLOCKS_PER_SEC<<endl;
    ofstream ofile("result/prediction.csv");
    ofile<<"Genome id"<<","<<"B.1.427"<<","<<"B.1.1.7"<<","<<"P.1"<<","<<"B.1.526"<<","<<"prediction"<<endl;
    ofstream ofiletime("result/consuing_time.csv");
    ofiletime<<"encrytion_time"<<","<<"computation_time"<<","<<"decryption_time"<<","<<"prediction_time"<<","<<"round trip"<<endl;
	double time_enc,time_fc,time_pre,time_dec;
    clock_t startenc, endenc , time_fc0_s,time_fc0_d,time_fc1_s,time_fc1_d,startdec,enddec,startpre,endpre;
//  enc
    startenc = clock();
    tLweKeyGen(tlwekey); //tlwe key
    int input_width =input[0].size();//256
    TLweSample *enc_input = new_TLweSample_array(d_input*2,tlweparams);
    EncTLWE(enc_input,input);
    endenc = clock();
    time_enc=(double)(endenc-startenc)/CLOCKS_PER_SEC;
    ofiletime<<time_enc<<",";
    cout<<"encrypt input data time:"<<time_enc<<endl;
//      the first layer
    time_fc0_s=clock();
    TLweSample *fc00 = new_TLweSample_array(d_fc0*2, tlweparams);//16*2
    fc00 = fully_connection(enc_input, fc0w,d_fc0);
    time_fc0_d=clock();
    time_fc=(double)(time_fc0_d-time_fc0_s)/CLOCKS_PER_SEC;
    cout<<"the first layer time:"<<time_fc<<",";
//      the second layer
    time_fc1_s=clock();
    TLweSample *fc11 = new_TLweSample_array(d_output*2, tlweparams);//4
    fc11 = fully_connection(fc00, fc1w ,d_output);
    time_fc1_d=clock();
    time_fc=(double)(time_fc1_d-time_fc1_s)/CLOCKS_PER_SEC;
    cout<<"the second layer time:"<<time_fc<<endl;
    time_fc=(double)(time_fc1_d-time_fc0_s)/CLOCKS_PER_SEC;
    cout<<"the computation time:"<<time_fc<<endl;
    ofiletime<<time_fc<<",";
//       dec
    startdec=clock();
    for(int i=0;i<8;i++){
        tLweSymDecrypt(dechif+i,fc11+i,tlwekey,spc_msg);
    }
    enddec =clock();
    time_dec=(double)(enddec-startdec)/CLOCKS_PER_SEC;
    cout<<"the dec time:"<<time_dec<<endl;
    ofiletime<<time_dec<<",";
//       predictions
    startpre =clock();
    for(int num =0; num<2000; num++)
    {
       ofile<<num<<",";
       vector<int> pre = Pre(dechif+(num<1000?0:4),num-(num<1000?0:1000));
       for(int i=0;i<4;i++){
          ofile<<pre[i]<<",";
       }
       int prediction =pre[4];
       string pred = Prints(prediction);
       ofile<<pred<<endl;
    }
    ofile.close();
    endpre=clock();
    double time_per = (double)(endpre-startpre)/CLOCKS_PER_SEC;
    ofiletime<<time_per<<",";
    cout<<"the prediction time:"<<time_per<<endl;
    double round_trip = time_enc+time_fc+time_dec;
    ofiletime<<round_trip<<endl;
    cout<<"the round trip time:"<<round_trip<<endl;
    ofiletime.close();
    delete_TLweSample_array(input_width*2,enc_input);
    delete_TLweSample_array(d_fc0*2, fc00);
    delete_TLweSample_array(d_output*2, fc11);
    delete_TorusPolynomial_array(d_output*2,dechif);
    cout<<"prediction over ~"<<endl;
    return 0;
}
