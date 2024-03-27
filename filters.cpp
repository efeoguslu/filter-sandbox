#include "filters.h"

void FirstOrderIIR_Init(FirstOrderIIR *filt, float alpha){

    if(alpha < 0.0f){
        filt->alpha = 0.0f;
    }
    else if(alpha > 1.0f){
        filt->alpha = 1.0f;
    }
    else{
        filt->alpha = alpha;
    }

    filt->out = 0.0f;
}


float FirstOrderIIR_Update(FirstOrderIIR *filt, float in){

    filt->out = (1.0f - filt->alpha) * in + filt->alpha * filt->out;
    return filt->out;
}

// ----------------------------------------------------------------------------------------------

void IFX_EMA_Init(IFX_EMA *filt, float alpha){

    IFX_EMA_SetAlpha(filt, alpha);

    filt->out = 0.0f;
}

void IFX_EMA_SetAlpha(IFX_EMA *filt, float alpha){

    if(alpha < 0.0f){
        filt->alpha = 0.0f;
    }
    else if(alpha > 1.0f){
        filt->alpha = 1.0f;
    }
    
    filt->alpha = alpha;   
}

float IFX_EMA_Update(IFX_EMA *filt, float in){
    filt->out = filt->alpha * in + (1.0f - filt->alpha)*filt->out;
    return filt->out;
}


//-----------------------------------------------------------------------------------------------

static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = {
-0.000227117658700331f,
-0.001196416074915810f,
-0.002801459039811921f,
-0.005158532332304008f,
-0.007636311356185004f,
-0.008743710796286766f,
-0.006386234785401823f,
0.001536075289928884f,
0.016355910703706587f,
0.037948912414491290f,
0.064387932368129688f,
0.092110326460397671f,
0.116606842228926699f,
0.133465600929534717f,
0.139476363296980072f,
0.133465600929534717f,
0.116606842228926699f,
0.092110326460397685f,
0.064387932368129702f,
0.037948912414491304f,
0.016355910703706591f,
0.001536075289928884f,
-0.006386234785401831f,
-0.008743710796286767f,
-0.007636311356185007f,
-0.005158532332304010f,
-0.002801459039811919f,
-0.001196416074915811f,
-0.000227117658700331f,
}; 

                                                            

void FIRFilter_Init(FIRFilter *fir){

    // Clear input buffer
    for(uint8_t n = 0; n < FIR_FILTER_LENGTH; ++n){
        fir->buf[n] = 0.0f;
    }

    // Reset buffer index
    fir->bufIndex = 0;

    // Clear filter output
    fir->out = 0.0f;
}


float FIRFilter_Update(FIRFilter *fir, float inp){

    // Store latest sample in buffer

    fir->buf[fir->bufIndex] = inp;

    // Increment buffer index and wrap around if necessary

    fir->bufIndex++;

    if(fir->bufIndex == FIR_FILTER_LENGTH){
        fir->bufIndex = 0;
    }

    // Compute new output sample (via Convolution)

    fir->out = 0.0f;

    uint8_t sumIndex = fir->bufIndex;

    for(uint8_t n = 0; n < FIR_FILTER_LENGTH; ++n){

        // Decrement index and wrap if necessary 

        if(sumIndex>0){
            sumIndex--;
        }
        else{
            sumIndex = FIR_FILTER_LENGTH - 1;
        }

        // Multiply impulse response with shifted input sample and add to output

        fir->out += FIR_IMPULSE_RESPONSE[n] * fir->buf[sumIndex];

    }

    // Return filtered output

    return fir->out;
}