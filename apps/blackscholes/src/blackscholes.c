// Copyright (c) 2007 Intel Corp.

// Black-Scholes
// Analytical method for calculating European Options
//
// 
// Reference Source: Options, Futures, and Other Derivatives, 3rd Edition, Prentice 
// Hall, John C. Hull,

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>


#ifdef NPU_FANN
#include "floatfann.h"
struct fann *ann ;
fann_type* parrotOut ;
#endif


#ifdef NPU_ANALOG
#include "neuralnetwork.hpp"
#include "xml_parser.hpp"
boost::shared_ptr<anpu::NeuralNetwork> currNeuralNetworkPtr ;
ApproxType at ;
int iBits ;
int wBits ;
#endif




double max_otype, min_otype ;
double max_sptprice, min_sptprice;
double max_strike, min_strike;
double max_rate, min_rate ;
double max_volatility, min_volatility;
double max_otime, min_otime ;
double max_out_price, min_out_price;


#define DIVIDE 120.0




//Precision to use for calculations
#define fptype float

#define NUM_RUNS 1

typedef struct OptionData_ {
        fptype s;          // spot price
        fptype strike;     // strike price
        fptype r;          // risk-free interest rate
        fptype divq;       // dividend rate
        fptype v;          // volatility
        fptype t;          // time to maturity or option expiration in years 
                           //     (1yr = 1.0, 6mos = 0.5, 3mos = 0.25, ..., etc)  
        char OptionType;   // Option type.  "P"=PUT, "C"=CALL
        fptype divs;       // dividend vals (not used in this test)
        fptype DGrefval;   // DerivaGem Reference Value
} OptionData;

OptionData *data;
fptype *prices;
int numOptions;

int    * otype;
fptype * sptprice;
fptype * strike;
fptype * rate;
fptype * volatility;
fptype * otime;
int numError = 0;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Cumulative Normal Distribution Function
// See Hull, Section 11.8, P.243-244
#define inv_sqrt_2xPI 0.39894228040143270286

fptype CNDF ( fptype InputX ) 
{
    int sign;

    fptype OutputX;
    fptype xInput;
    fptype xNPrimeofX;
    fptype expValues;
    fptype xK2;
    fptype xK2_2, xK2_3;
    fptype xK2_4, xK2_5;
    fptype xLocal, xLocal_1;
    fptype xLocal_2, xLocal_3;

    // Check for negative value of InputX
    if (InputX < 0.0) {
        InputX = -InputX;
        sign = 1;
    } else 
        sign = 0;

    xInput = InputX;
 
    // Compute NPrimeX term common to both four & six decimal accuracy calcs
    expValues = exp(-0.5f * InputX * InputX);
    xNPrimeofX = expValues;
    xNPrimeofX = xNPrimeofX * inv_sqrt_2xPI;

    xK2 = 0.2316419 * xInput;
    xK2 = 1.0 + xK2;
    xK2 = 1.0 / xK2;
    xK2_2 = xK2 * xK2;
    xK2_3 = xK2_2 * xK2;
    xK2_4 = xK2_3 * xK2;
    xK2_5 = xK2_4 * xK2;
    
    xLocal_1 = xK2 * 0.319381530;
    xLocal_2 = xK2_2 * (-0.356563782);
    xLocal_3 = xK2_3 * 1.781477937;
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_4 * (-1.821255978);
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_5 * 1.330274429;
    xLocal_2 = xLocal_2 + xLocal_3;

    xLocal_1 = xLocal_2 + xLocal_1;
    xLocal   = xLocal_1 * xNPrimeofX;

    //printf("# xLocal: %10.10f\n", xLocal);



    xLocal   = 1.0 - xLocal;

    OutputX  = xLocal;

    //printf("# Output: %10.10f\n", OutputX);
    
    if (sign) {
        OutputX = 1.0 - OutputX;
    }
    
    return OutputX;
} 

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
fptype BlkSchlsEqEuroNoDiv( fptype sptprice,
                            fptype strike, fptype rate, fptype volatility,
                            fptype time, int otype, float timet, fptype* N1, fptype* N2)
{
    fptype OptionPrice;

    // local private working variables for the calculation
    //fptype xStockPrice;
    //fptype xStrikePrice;
    fptype xRiskFreeRate;
    fptype xVolatility;
    fptype xTime;
    fptype xSqrtTime;

    fptype logValues;
    fptype xLogTerm;
    fptype xD1; 
    fptype xD2;
    fptype xPowerTerm;
    fptype xDen;
    fptype d1;
    fptype d2;
    fptype FutureValueX;
    fptype NofXd1;
    fptype NofXd2;
    fptype NegNofXd1;
    fptype NegNofXd2;  
    
    //xStockPrice = sptprice;
    //xStrikePrice = strike;
    xRiskFreeRate = rate;
    xVolatility = volatility;
    xTime = time;


    xSqrtTime = sqrt(xTime);

    logValues = log( sptprice / strike );
        
    xLogTerm = logValues;
        
    
    xPowerTerm = xVolatility * xVolatility;
    xPowerTerm = xPowerTerm * 0.5;
        
    xD1 = xRiskFreeRate + xPowerTerm;
    xD1 = xD1 * xTime;
    xD1 = xD1 + xLogTerm;

    

    xDen = xVolatility * xSqrtTime;
    xD1 = xD1 / xDen;
    xD2 = xD1 -  xDen;

    d1 = xD1;
    d2 = xD2;
    
    NofXd1 = CNDF( d1 );

    if(NofXd1 > 1.0)
        std::cerr << "Greater than one!" << std::endl ;
    //printf("# d1: %10.10f\n", NofXd1);

    NofXd2 = CNDF( d2 );
    if(NofXd2 > 1.0)
         std::cerr << "Greater than one!" << std::endl ;
    //printf("# d2: %10.10f\n", NofXd2);

    *N1 = NofXd1 ;
    *N2 = NofXd2 ;

    FutureValueX = strike * ( exp( -(rate)*(time) ) );        
    if (otype == 0) {            
        OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
        
    } else { 
        NegNofXd1 = (1.0 - NofXd1);
        NegNofXd2 = (1.0 - NofXd2);
        OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
    }
    
    return OptionPrice;
}


double normalize(double in, double min, double max, double min_new, double max_new)
{
    return (((in - min) / (max - min)) * (max_new - min_new)) + min_new ;
}

int bs_thread(void *tid_ptr) {
    int i, j;
    //fptype price0, price1;
    //fptype priceDelta;
    int tid = *(int *)tid_ptr;
    int start = tid * (numOptions);
    int end = start + (numOptions);

    //printf("%d\n", (end-start) * NUM_RUNS);
    //printf("%d\n", 6);
    //printf("%d\n", 1);



    #ifdef NPU_ANALOG
        std::ofstream sw_diff ;
        sw_diff.open("./data/npu_analog_diff.data");
        sw_diff << "Price_Orig\tPriceBlackscholes\tPrice_NN\n";
        int count_error = 0;
    #endif

    #ifdef NPU_FANN
        std::ofstream fann_diff ;
        fann_diff.open("./data/npu_fann_diff.data") ;
        printf("# Creating the NN from the FANN configuration file...\n");
        fann_diff << "Price_Orig\tPriceBlackscholes\tPrice_NN\n";
    #endif


    #ifdef NPU_OBSERVATION
        std::ofstream fann_observation;
        fann_observation.open("./train/blackscholes.data") ;
        fann_observation << "48305\n6\n1\n";
    #endif



int count_nn_errors = 0;
int count_nn_total  = 0;

    for (j=0; j<NUM_RUNS; j++) {
        for (i=start; i<end; i++) {
            /* Calling main function to calculate option value based on 
             * Black & Scholes's equation.
             */
             #ifdef NPU_ANALOG
                std::vector<double> inputData ;
                std::vector<double> outputData ;
                fptype price_orig, price_nn ;
                 fptype N1,  N2 ;


                inputData.push_back(sptprice[i]);
                inputData.push_back(strike[i]);
                inputData.push_back(rate[i]);
                inputData.push_back(volatility[i]);
                inputData.push_back(otime[i]);
                inputData.push_back(otype[i]);


                // // for(int i = 0 ; i < 6; i++)
                // // {
                // //     std::cout << "# Data: " << inputData[i] << std::endl;
                // // }

                currNeuralNetworkPtr->runNeuralNetwork(inputData, outputData, iBits, wBits, at) ;

                // fptype N1, N2 ;

                price_orig = BlkSchlsEqEuroNoDiv(sptprice[i], strike[i],
                                         rate[i], volatility[i], otime[i], 
                                         otype[i], 0, &N1, &N2);


                sw_diff << data[i].DGrefval << "\t" ;
                sw_diff << price_orig * DIVIDE << "\t";
                sw_diff << outputData[0] * DIVIDE << "\n";

             #endif


            #ifdef NPU_FANN
                fptype price_orig, price_nn;

                fptype N1,  N2 ;

                //fptype price_0, price_1;

                price_orig = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
                                         rate[i], volatility[i], otime[i], 
                                         otype[i], 0, &N1, &N2);



                // if(rate[i] != data[i].r)
                // {
                //     std::cerr << "There is an errro!" << std::endl ;
                //     exit(1);
                // }


                // fptype bs_type = 0.0 ;

                // if(otype[i] == 0.0)
                // {
                //     bs_type = -1.0;
                // }
                // else
                // {
                //     bs_type = 1.0;
                // }


                fptype datain[6] = {    sptprice[i],
                                        strike[i],
                                        rate[i],
                                        volatility[i],
                                        otime[i],
                                        (float)otype[i]
                                    } ;

                parrotOut = fann_run(ann, (fann_type*)datain) ;
                price_nn = parrotOut[0];


                if((price_nn * DIVIDE - data[i].DGrefval) * (price_nn *  DIVIDE - data[i].DGrefval) > 0.01)
                {
                    count_nn_errors++ ;
                }
               
                count_nn_total++;


                fann_diff << data[i].DGrefval << "\t" ;
                fann_diff << price_orig * DIVIDE << "\t";
                fann_diff << price_nn * DIVIDE << "\n";
                

            #endif

             

            #ifdef NPU_OBSERVATION

                    fann_observation << std::setprecision(4) << sptprice[i] << " " ;
                    fann_observation << std::setprecision(4) << strike[i] << " " ;
                    fann_observation << std::setprecision(4) << rate[i] << " " ;
                    fann_observation << std::setprecision(4) << volatility[i] << " " ;
                    fann_observation << std::setprecision(4) << otime[i]  << " ";
                    fann_observation << std::setprecision(4) << otype[i] << std::endl;
                    


                fptype N1, N2 ;
                fptype price ;

                price = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
                                         rate[i], volatility[i], otime[i], 
                                         otype[i], 0, &N1, &N2);

                    
                fann_observation << std::setprecision(8) << price  << std::endl ;

                prices[i] = price;

            #endif

#ifdef ERR_CHK
        #ifdef NPU_FANN
            priceDelta = data[i].DGrefval - price_orig * (double)data[i].strike ;
            if( fabs(priceDelta) >= 1e-4 ){
                printf("Error on %d. Computed=%.5f, Ref=%.5f, Delta=%.5f\n",
                       i, price, data[i].DGrefval, priceDelta);
                numError ++;
            }
        #endif
        #ifdef NPU_ANALOG
            priceDelta = data[i].DGrefval - price_orig * (double)data[i].strike ;
            if( fabs(priceDelta) >= 1e-4 ){
                printf("Error on %d. Computed=%.5f, Ref=%.5f, Delta=%.5f\n",
                       i, price, data[i].DGrefval, priceDelta);
                numError ++;
            }
        #endif
#endif
        }
    }


    #ifdef NPU_ANALOG
        sw_diff.close();
    #endif

    #ifdef NPU_FANN
        //std::cout << "# Total: " << count_nn_total << std::endl ;
        //std::cout << "# Error: " << count_nn_errors / (double) count_nn_total << std::endl ;
        //fann_diff.close();
    #endif

    #ifdef NPU_OBSERVATION
        fann_observation.close() ;
        //fann_observation1.close() ;
    #endif





    // std::cout << "# Total Run:      " << countTotal << std::endl ;
    //std::cout << "# Absolute Error: " << absError / (double)count << std::endl ;
    // std::cout << "# Miss:           " << countErrors / (double)countTotal << std::endl;

    return 0;
}

int main (int argc, char **argv)
{
    FILE *file;
    int i;
    int loopnum;
    fptype * buffer;
    int * buffer2;
    int rv;


	fflush(NULL);


    char *inputFile = argv[1];
    char *outputFile = argv[2];

    #ifdef NPU_ANALOG
        std::string nn_name = argv[3] ;
        iBits = atoi(argv[4]) ;
        wBits = atoi(argv[5]) ;
        int isFp  = atoi(argv[6]) ;
        if(isFp == 0)
        {
            at = LP ;
        }
        else
        {
            at = FP ;
        }
        anpu::XMLParser currXMLParser(nn_name, currNeuralNetworkPtr, false) ;
    #endif


    #ifdef NPU_FANN
        std::string nn = argv[3] ;
        ann = fann_create_from_file(nn.c_str()) ;
    #endif

    //Read input data from file
    file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }
    rv = fscanf(file, "%i", &numOptions);
    if(rv != 1) {
      printf("ERROR: Unable to read from file `%s'.\n", inputFile);
      fclose(file);
      exit(1);
    }


    // alloc spaces for the option data
    data = (OptionData*)malloc(numOptions*sizeof(OptionData));
    prices = (fptype*)malloc(numOptions*sizeof(fptype));
    for ( loopnum = 0; loopnum < numOptions; ++ loopnum )
    {
        rv = fscanf(file, "%f %f %f %f %f %f %c %f %f", &data[loopnum].s, &data[loopnum].strike, &data[loopnum].r, &data[loopnum].divq, &data[loopnum].v, &data[loopnum].t, &data[loopnum].OptionType, &data[loopnum].divs, &data[loopnum].DGrefval);
        if(rv != 9) {
          printf("ERROR: Unable to read from file `%s'.\n", inputFile);
          fclose(file);
          exit(1);
        }
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", inputFile);
      exit(1);
    }

    //printf("Num of Options: %d\n", numOptions);
    //printf("Num of Runs: %d\n", NUM_RUNS);

#define PAD 256
#define LINESIZE 64

    buffer = (fptype *) malloc(5 * numOptions * sizeof(fptype) + PAD);
    sptprice = (fptype *) (((unsigned long long)buffer + PAD) & ~(LINESIZE - 1));
    strike = sptprice + numOptions;
    rate = strike + numOptions;
    volatility = rate + numOptions;
    otime = volatility + numOptions;

    buffer2 = (int *) malloc(numOptions * sizeof(fptype) + PAD);
    otype = (int *) (((unsigned long long)buffer2 + PAD) & ~(LINESIZE - 1));



    



    bool isFirst = true ;

    for (i=0; i<numOptions; i++) {
        otype[i]      = (data[i].OptionType == 'P') ? 1 : 0;
        //sptprice[i]   = data[i].s;
        sptprice[i]   = data[i].s / DIVIDE;
        //strike[i]     = data[i].strike;
        strike[i]     = data[i].strike / DIVIDE;
        //std::cout << "strike: " << strike[i] << std::endl ;
        rate[i]       = data[i].r;
        volatility[i] = data[i].v;    
        otime[i]      = data[i].t;


        if(isFirst)
        {
            max_sptprice = sptprice[i] ;
            min_sptprice = sptprice[i] ;

            max_strike = strike[i] ;
            min_strike = strike[i] ;

            min_rate   = rate[i] ;
            max_rate    = rate[i] ;

            min_volatility = volatility[i] ;
            max_volatility = volatility[i] ;

            max_otime = otime[i] ;
            min_otime = otime[i] ;

            max_out_price = data[i].DGrefval;
            min_out_price = data[i].DGrefval;
            isFirst = false;
        }
        else
        {

            if(sptprice[i] > max_sptprice)
            {
                max_sptprice = sptprice[i] ;
            }
            if(sptprice[i] < min_sptprice)
            {
                min_sptprice = sptprice[i] ;
            }

            if(strike[i] > max_strike)
            {
                max_strike = strike[i] ;
            }
            if(strike[i] < min_strike)
            {
                min_strike = strike[i] ;
            }

            if(rate[i] > max_rate)
            {
                max_rate = rate[i] ;
            }
            if(rate[i] < min_rate)
            {
                min_rate = rate[i] ;
            }

            if(volatility[i] > max_volatility)
            {
                max_volatility = volatility[i] ;
            }
            if(volatility[i] < min_volatility)
            {
                min_volatility = volatility[i] ;
            }

            if(otime[i] > max_otime)
            {
                max_otime = otime[i] ;
            }
            if(otime[i] < min_otime)
            {
                min_otime = otime[i] ;
            }

            if(data[i].DGrefval > max_out_price)
            {
                max_out_price = data[i].DGrefval;
            }
            if(data[i].DGrefval < min_out_price)
            {
                min_out_price = data[i].DGrefval;
            }
        }
    }


    max_otype = 1.0;
    min_otype = 0.0 ;




    //printf("Size of data: %d\n", numOptions * (sizeof(OptionData) + sizeof(int)));

    //serial version
    int tid=0;
    bs_thread(&tid);


    //Write prices to output file
    file = fopen(outputFile, "w");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", outputFile);
      exit(1);
    }
    rv = fprintf(file, "%i\n", numOptions);
    if(rv < 0) {
      printf("ERROR: Unable to write to file `%s'.\n", outputFile);
      fclose(file);
      exit(1);
    }
    for(i=0; i<numOptions; i++) {
      rv = fprintf(file, "%.18f\n", prices[i]);
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(file);
        exit(1);
      }
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", outputFile);
      exit(1);
    }

// #ifdef ERR_CHK
//     printf("Num Errors: %d\n", numError);
// #endif
    free(data);
    free(prices);

    return 0;
}

