#ifndef _FPU_FILTER_H_
#define _FPU_FILTER_H_
//#############################################################################
//! \file   include/fpu_filter.h
//!
//! \brief  Prototypes and Definitions for the C28x FPU Library
//! \author Vishal Coelho
//! \date   n/a
//
//  Group: 			C2000
//  Target Family:	F2837x
//
// Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
// ALL RIGHTS RESERVED 
//#############################################################################
//$TI Release: C28x Floating Point Unit Library V1.50.00.00 $
//$Release Date: Jun 2, 2015 $
//#############################################################################

//*****************************************************************************
// includes
//*****************************************************************************
#include "fpu_types.h"

//!
//! \defgroup fpu_filter FIR Filters

//!
//! \addtogroup fpu_filter
// @{ 

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
// defines
//*****************************************************************************
#define NULL    0

//*****************************************************************************
// typedefs
//*****************************************************************************
//! Structure for the Finite Impulse Response Filter
typedef struct { 
    float *coeff_ptr;        //!<  Pointer to Filter coefficient
    float *dbuffer_ptr;      //!<  Delay buffer pointer
    int	cbindex;			 //!<  Circular Buffer Index
    int order;               //!<  Order of the Filter
    float input;             //!<  Latest Input sample
    float output;            //!<  Filter Output
    void (*init)(void *);    //!<  Pointer to Initialization function
    void (*calc)(void *);    //!<  Pointer to the calculation function
    }FIR_FP;

//! Handle to the Filter Structure Object
typedef FIR_FP 	*FIR_FP_Handle;


#define FIR_FP_DEFAULTS { (float *)NULL, \
             (float *)NULL,  \
             0,              \
             50,             \
             0,				 \
             0,				 \
             (void (*)(void *))FIR_FP_init,\
             (void (*)(void *))FIR_FP_calc}    

//*****************************************************************************
// function prototypes
//*****************************************************************************
//! \brief Finite Impulse Response Filter.
//!
//! This routine implements the non-recursive difference equation of an
//! all-zero filter (FIR), of order N. All the coefficients of all-zero filter
//! are assumed to be less than 1 in magnitude.
//! \param hndFIR_FP Handle to the FIR_FP object
//! \attention
//! -# The delay and coefficients buffer must be aligned to a minimum of 
//! 2 x (order + 1) words.
//! For example, if the filter order is 31, it will have 32 taps or 
//! coefficients each a 32-bit floating point value. A minimum of 
//! (2 * 32) = 64 words will need to be allocated for the delay and 
//! coefficients buffer.
//! -# To align the buffer, use the DATA_SECTION pragma to assign the buffer to
//! a code section and then align the buffer to the proper offset in the linker
//! command file. In the code example the buffer is assigned to the \b firldb 
//! section while the coefficients are assigned to the \b coefffilt section.
//! -# This routine requires the --c2xlp_src_compatible option to be enabled 
//! in the file specific properties
extern void FIR_FP_calc(FIR_FP_Handle hndFIR_FP);

//! \brief Finite Impulse Response Filter Initialization.
//!
//! Zeros out the delay line 
//! \param hndFIR_FP Handle to the FIR_FP object
//! \attention
//! -# The delay and coefficients buffer must be aligned to a minimum of 
//! 2 x (order + 1) words.
//! For example, if the filter order is 31, it will have 32 taps or 
//! coefficients each a 32-bit floating point value. A minimum of (2 * 32) = 64 
//! words will need to be allocated for the delay and coefficients buffer.
//! -# The delay buffer needs to be aligned to word boundary of 2 * number of 
//! taps
//! -# To align the buffer, use the DATA_SECTION pragma to assign the buffer to
//! a code section and then align the buffer to the proper offset in the linker
//! command file. In the code example the buffer is assigned to the \b firldb
//! section while the coefficients are assigned to the \b coefffilt section.
extern void FIR_FP_init(FIR_FP_Handle hndFIR_FP);

// @} //addtogroup

/*********** Sample FIR Co-efficients **************************/

/* 5th order LPF co-efficients for FIR_FP module	*/
#define FIR_FP_LPF6 {\
  -0.08057276905, 0.1966465116, 0.4776741266, 0.4776741266, 0.1966465116,\
  -0.08057276905}

/* 31st order LPF co-efficients for FIR_FP module	*/	
#define FIR_FP_LPF32 {\
  -0.0009375925292,-0.001849809778, 0.002820055233, 0.003732349491,-0.005677098874,\
  -0.007562109735,  0.01061980333,  0.01369810477, -0.01852593757, -0.02375750057,\
   0.03181955963,  0.04181841016, -0.05826609582, -0.08511561155,   0.1475527734,\
   0.4489050806,   0.4489050806,   0.1475527734, -0.08511561155, -0.05826609582,\
   0.04181841016,  0.03181955963, -0.02375750057, -0.01852593757,  0.01369810477,\
   0.01061980333,-0.007562109735,-0.005677098874, 0.003732349491, 0.002820055233,\
  -0.001849809778,-0.0009375925292}

/* 50th order LPF co-efficients for FIR_FP module	*/
#define FIR_FP_LPF50 {\
  -0.0007520393119,-0.003496379592,-0.002576870145,-0.002954199212,-0.001902273274, \
  7.578533405e-05, 0.002921634587, 0.006043223664, 0.008603606373, 0.009631498717,  \
   0.008286305703, 0.004132356495,-0.002609857591,  -0.0109326588, -0.01908121072,  \
   -0.02479325049, -0.02570373565,    -0.01984559,-0.006156369112,  0.01515351236,  \
    0.04245631397,  0.07283417881,   0.1024993882,   0.1274179369,   0.1440151036,  \
     0.1498377919,   0.1440151036,   0.1274179369,   0.1024993882,  0.07283417881,  \
    0.04245631397,  0.01515351236,-0.006156369112,    -0.01984559, -0.02570373565,  \
   -0.02479325049, -0.01908121072,  -0.0109326588,-0.002609857591, 0.004132356495,  \
   0.008286305703, 0.009631498717, 0.008603606373, 0.006043223664, 0.002921634587,  \
  7.578533405e-05,-0.001902273274,-0.002954199212,-0.002576870145,-0.003496379592,  \
  -0.0007520393119}

/* 63rd order LPF co-efficients for FIR_FP module	*/	
#define FIR_FP_LPF64 {\
    0.0128396498, 0.002229209524, 0.001987809548, 0.001418562024,0.0005116602406,\
   -0.0007191105396,-0.002233661246,-0.003967377357,-0.005827924237,-0.007696942892,\
   -0.009434565902,  -0.0108908657, -0.01190613955, -0.01232818887, -0.01201683283,\
   -0.01084406395,-0.008708032779,-0.005558374338, -0.00141107582, 0.003738404717,\
    0.009769263677,  0.01655653119,  0.02391541377,  0.03162577376,  0.03943601623,\
    0.04707739502,  0.05427397415,  0.06076014042,  0.06628206372,  0.07063338161,\
    0.07363332808,  0.07516146451,  0.07516146451,  0.07363332808,  0.07063338161,\
    0.06628206372,  0.06076014042,  0.05427397415,  0.04707739502,  0.03943601623,\
    0.03162577376,  0.02391541377,  0.01655653119, 0.009769263677, 0.003738404717,\
   -0.00141107582,-0.005558374338,-0.008708032779, -0.01084406395, -0.01201683283,\
   -0.01232818887, -0.01190613955,  -0.0108908657,-0.009434565902,-0.007696942892,\
   -0.005827924237,-0.003967377357,-0.002233661246,-0.0007191105396,0.0005116602406,\
    0.001418562024, 0.001987809548, 0.002229209524,   0.0128396498}
	
	
/* 127th order LPF co-efficients for FIR_FP module	*/	
#define FIR_FP_LPF128 {\
  -5.036197308e-007,-8.415498996e-006,-1.034940033e-005,-5.885554401e-006,\
   9.191019672e-006,2.868542833e-005,3.739785097e-005,1.950809019e-005,-2.699213837e-005,\
  -8.004013944e-005,-9.904401668e-005,-4.961189916e-005,6.434905663e-005,\
   0.0001849089895,0.0002214506821,0.0001080058428,-0.0001343335607,-0.0003775364021,\
  -0.0004417464952,-0.0002112832153,0.0002549774072,0.0007046277751,0.0008100091363,\
   0.0003816168755,-0.0004500565119,-0.001227322151,-0.001391345286,-0.0006477040588,\
   0.0007501002983,  0.00202401611, 0.002269187942, 0.001046379562,-0.001194330398,\
  -0.003196024569,-0.003552211681,-0.001626144163, 0.001834984054, 0.004880517256,\
   0.005390366074, 0.002455403563,-0.002747559687,-0.007281318307,-0.008013337851,\
  -0.003642429598, 0.004055963829,  0.01074570417,  0.01182830054, 0.005387275945,\
  -0.005999655928, -0.01597604528, -0.01769949682,-0.008136766963, 0.009144407697,\
   0.02473383211,  0.02794272266,  0.01318075042, -0.01527078357, -0.04315049946,\
  -0.05166593194, -0.02643037774,   0.0343558751,   0.1168198511,   0.1955456734,\
   0.2435260415,   0.2435260415,   0.1955456734,   0.1168198511,   0.0343558751,\
  -0.02643037774, -0.05166593194, -0.04315049946, -0.01527078357,  0.01318075042,\
   0.02794272266,  0.02473383211, 0.009144407697,-0.008136766963, -0.01769949682,\
  -0.01597604528,-0.005999655928, 0.005387275945,  0.01182830054,  0.01074570417,\
   0.004055963829,-0.003642429598,-0.008013337851,-0.007281318307,-0.002747559687,\
   0.002455403563, 0.005390366074, 0.004880517256, 0.001834984054,-0.001626144163,\
  -0.003552211681,-0.003196024569,-0.001194330398, 0.001046379562, 0.002269187942,\
   0.00202401611,0.0007501002983,-0.0006477040588,-0.001391345286,-0.001227322151,\
  -0.0004500565119,0.0003816168755,0.0008100091363,0.0007046277751,0.0002549774072,\
  -0.0002112832153,-0.0004417464952,-0.0003775364021,-0.0001343335607,0.0001080058428,\
   0.0002214506821,0.0001849089895,6.434905663e-005,-4.961189916e-005,-9.904401668e-005,\
  -8.004013944e-005,-2.699213837e-005,1.950809019e-005,3.739785097e-005,2.868542833e-005,\
   9.191019672e-006,-5.885554401e-006,-1.034940033e-005,-8.415498996e-006,\
  -5.036197308e-007}


/* 255th order LPF co-efficients for FIR_FP module*/
#define FIR_FP_LPF256 {\
 -1.80824869e-010,-8.315279021e-010,-1.350614176e-009,-8.081277314e-010,\
  1.887117351e-009,6.140868702e-009,8.732807366e-009,4.66826533e-009,-8.818096475e-009,\
 -2.704159563e-008,-3.596992215e-008,-1.852767539e-008,3.084824485e-008,\
  9.168023496e-008,1.172344284e-007,5.897494049e-008,-9.044957494e-008,-2.631300049e-007,\
 -3.272649565e-007,-1.617399334e-007,2.340127452e-007,6.696759556e-007,8.153773479e-007,\
  3.972507443e-007,-5.500381235e-007,-1.553042466e-006,-1.858763198e-006,\
 -8.947562264e-007,1.196682433e-006,3.340597232e-006, 3.9412771e-006,1.877603609e-006,\
 -2.441214747e-006,-6.747715815e-006,-7.863823157e-006,-3.712174021e-006,\
  4.713714134e-006,1.29157479e-005,1.489149327e-005,6.972494702e-006,-8.676702237e-006,\
 -2.358927668e-005,-2.694060095e-005,-1.252162838e-005,1.531135604e-005,\
  4.13336129e-005,4.680583515e-005,2.16096505e-005,-2.60195302e-005,-6.979000318e-005,\
 -7.842414925e-005,-3.598654439e-005,4.273937157e-005,0.0001139624583,0.0001271686488,\
  5.802695523e-005,-6.807099999e-005,-0.0001805275242,-0.0002001633402,-9.086308273e-005,\
  0.0001054084642, 0.000278158026,0.0003066107165,0.0001385226351,-0.0001590753091,\
 -0.0004178562667,-0.000458129216,-0.0002060718834,0.0002344652166,0.0006133037386,\
  0.0006691126619,0.0002997705888,-0.0003381973947,-0.000881257467,-0.0009571494302,\
 -0.0004272586957,0.0004783110344, 0.001242064871, 0.001343587413,0.0005978165427,\
 -0.0006645498215,-0.001720438944,-0.001854412374,-0.0008227802464,0.0009088313673,\
  0.00234676525, 0.002521753777, 0.001116263331,-0.001226079883,-0.003159437329,\
 -0.003386604367,-0.001496470417, 0.001635757973, 0.004209185485, 0.004503889009,\
  0.001988161588,-0.002164767357,-0.005567329004,-0.005952220876, -0.00262744003,\
  0.002853144193, 0.007342177909, 0.007853539661, 0.003471534234,-0.003765888279,\
 -0.009713715874, -0.01041548885,-0.004620380234, 0.005019695498,  0.01301413029,\
  0.0140327597,   0.0062699439,-0.006851418409, -0.01794240624, -0.01956933923,\
 -0.008868219331,  0.00982964132,  0.02626995556,  0.02935423329,  0.01371181197,\
 -0.01574329473, -0.04415137321, -0.05252153799, -0.02672382817,  0.03458277881,\
  0.1172149479,   0.1957845688,   0.2435607016,   0.2435607016,   0.1957845688,\
  0.1172149479,  0.03458277881, -0.02672382817, -0.05252153799, -0.04415137321,\
 -0.01574329473,  0.01371181197,  0.02935423329,  0.02626995556,  0.00982964132,\
 -0.008868219331, -0.01956933923, -0.01794240624,-0.006851418409,   0.0062699439,\
  0.0140327597,  0.01301413029, 0.005019695498,-0.004620380234, -0.01041548885,\
 -0.009713715874,-0.003765888279, 0.003471534234, 0.007853539661, 0.007342177909,\
  0.002853144193, -0.00262744003,-0.005952220876,-0.005567329004,-0.002164767357,\
  0.001988161588, 0.004503889009, 0.004209185485, 0.001635757973,-0.001496470417,\
 -0.003386604367,-0.003159437329,-0.001226079883, 0.001116263331, 0.002521753777,\
  0.00234676525,0.0009088313673,-0.0008227802464,-0.001854412374,-0.001720438944,\
 -0.0006645498215,0.0005978165427, 0.001343587413, 0.001242064871,0.0004783110344,\
 -0.0004272586957,-0.0009571494302,-0.000881257467,-0.0003381973947,0.0002997705888,\
  0.0006691126619,0.0006133037386,0.0002344652166,-0.0002060718834,-0.000458129216,\
 -0.0004178562667,-0.0001590753091,0.0001385226351,0.0003066107165, 0.000278158026,\
  0.0001054084642,-9.086308273e-005,-0.0002001633402,-0.0001805275242,-6.807099999e-005,\
  5.802695523e-005,0.0001271686488,0.0001139624583,4.273937157e-005,-3.598654439e-005,\
 -7.842414925e-005,-6.979000318e-005,-2.60195302e-005,2.16096505e-005,4.680583515e-005,\
  4.13336129e-005,1.531135604e-005,-1.252162838e-005,-2.694060095e-005,-2.358927668e-005,\
 -8.676702237e-006,6.972494702e-006,1.489149327e-005,1.29157479e-005,4.713714134e-006,\
 -3.712174021e-006,-7.863823157e-006,-6.747715815e-006,-2.441214747e-006,\
  1.877603609e-006, 3.9412771e-006,3.340597232e-006,1.196682433e-006,-8.947562264e-007,\
 -1.858763198e-006,-1.553042466e-006,-5.500381235e-007,3.972507443e-007,\
  8.153773479e-007,6.696759556e-007,2.340127452e-007,-1.617399334e-007,-3.272649565e-007,\
 -2.631300049e-007,-9.044957494e-008,5.897494049e-008,1.172344284e-007,9.168023496e-008,\
  3.084824485e-008,-1.852767539e-008,-3.596992215e-008,-2.704159563e-008,\
 -8.818096475e-009,4.66826533e-009,8.732807366e-009,6.140868702e-009,1.887117351e-009,\
 -8.081277314e-010,-1.350614176e-009,-8.315279021e-010,-1.80824869e-010}

/* 511th order LPF co-efficients for FIR_FP module	*/	
#define FIR_FP_LPF512 {\
   0.001536677242,-0.0003126577358,-0.0003662098607,-0.0004262450675,-0.0004455255694,\
  -0.000398971024,-0.0002846919233,-0.000135702925,2.40697068e-006,7.971248851e-005,\
   7.27102597e-005,-1.486802466e-005,-0.0001417729945,-0.0002547842741,-0.0002992405789,\
  -0.000252182188,-0.0001221083512,4.165977953e-005, 0.000179249153,0.0002319578343,\
   0.0001801131148,3.767904491e-005,-0.0001344568736,-0.0002677424927,-0.0003065310011,\
  -0.0002275148436,-5.725489245e-005,0.0001380053436,0.0002811293816, 0.000310628966,\
   0.000211613733,1.823118509e-005,-0.0001922601077,-0.0003356365487,-0.0003484986664,\
  -0.0002217289875,2.793246949e-007,0.0002289074473,0.0003727058938,0.0003679615038,\
   0.0002126409963,-3.707793076e-005,-0.0002811307495,-0.0004207049788,-0.0003939556773,\
  -0.0002059252147,7.397215086e-005,0.0003325725556,0.0004658395774,0.0004126331769,\
   0.0001897966868,-0.000119867349,-0.0003910067317,-0.0005134353996,-0.0004297963169,\
  -0.0001687666081,0.0001713849924,0.0004527117708,0.0005602184101,0.0004419854668,\
   0.0001401899353,-0.000230337726,-0.0005189483054,-0.0006068443181,-0.0004493572924,\
  -0.0001041156211,0.0002965501626,0.0005886628642,0.0006519903545,0.0004506692349,\
   5.979820708e-005,-0.0003695303749,-0.000661567552,-0.0006946527865,-0.0004447723913,\
  -6.337595551e-006,0.0004500737414,0.0007371228421,0.0007341686287,0.0004310507502,\
  -5.656403664e-005,-0.0005378419883,-0.0008145574247,-0.0007692913059,-0.0004082531668,\
   0.0001296115515,0.0006328151794,0.0008932650671,0.0007992550964,0.0003759867686,\
  -0.0002129065397,-0.0007343280013,-0.0009722104296,-0.0008226116188,-0.0003326688602,\
   0.0003070131061,0.0008423305699, 0.001050422317,0.0008382110973,0.0002774739987,\
  -0.0004125079431,-0.0009565392975,-0.001127045951,-0.0008449512534,-0.0002096275712,\
   0.0005294296425, 0.001076267334, 0.001200915431,0.0008415097836,0.0001281714503,\
  -0.0006580534973,-0.001200831728,-0.001270573004,-0.0008263729396,-3.193271186e-005,\
   0.0007984013646, 0.001329447026, 0.001334693399,0.0007979645161,-8.005859854e-005,\
  -0.0009507808718,-0.001461334876,-0.001391842263,-0.0007549041766,0.0002086951135,\
   0.001115191495, 0.001595652779, 0.001440615626,0.0006958136219,-0.0003546522348,\
  -0.001291149412,-0.001730839838,-0.001478790655,-0.0006186420214,0.0005193172838,\
   0.001478948514, 0.001866118633, 0.001505041961,0.0005218694569,-0.0007033856236,\
  -0.001678185421,-0.002000011271,-0.001517228317,-0.0004036253085,0.0009079848533,\
   0.001888709841, 0.002131114481, 0.001513223164,0.0002619158186,-0.001134157064,\
  -0.002110183705,-0.002257819287,-0.001490679104,-9.448102355e-005,  0.00138323626,\
   0.002342519118, 0.002378498437, 0.001447157119,-0.0001011107233, -0.00165657443,\
  -0.002585405251,-0.002491400577,-0.001379889785,0.0003274458868, 0.001955616521,\
   0.002838404151, 0.002594103571, 0.001285271137,-0.0005881907418,-0.002282862319,\
  -0.00310203596,-0.002684962004,-0.001160169952, 0.000886691967, 0.002640358405,\
   0.003376020817, 0.002761132317, 0.001000010176,-0.001227727043,-0.003031382803,\
  -0.003660683753, -0.00281974976,-0.0007993766922, 0.001617376809,  0.00346039352,\
   0.003957144916, 0.002857826184,0.0005521377316,-0.002062810585,-0.003932794556,\
  -0.004266605712,-0.002871319419,-0.0002499786206, 0.002573836362, 0.004456216935,\
   0.004591232166, 0.002855427563,-0.0001178953898,-0.003163688118,-0.005041435361,\
  -0.004934640136,-0.002804262796,0.0005657746806, 0.003850434208, 0.005703321658,\
   0.005302053411, 0.002709657419,-0.001113970415, -0.00466015283,-0.006463991944,\
  -0.005701822229,-0.002560514258, 0.001791732735, 0.005631223787, 0.007356632967,\
   0.0061470652, 0.002340652747,-0.002643338637,-0.006822787691,-0.008433135226,\
  -0.006658871658,-0.002024828456, 0.003739544423, 0.008330924436,   0.0097791804,\
   0.007273038849, 0.001571267494,-0.005200890824, -0.01032224204, -0.01154625975,\
  -0.008054497652,-0.0009047139902, 0.007250499446,  0.01311222371,  0.01402795315,\
   0.009132681414,-0.0001267225889, -0.01035293471, -0.01737787947,  -0.0178749226,\
  -0.01080497075, 0.001884402242,  0.01566235721,  0.02488196269,  0.02487257868,\
   0.01393784769,-0.005489115138, -0.02704929188, -0.04209024832, -0.04230293259,\
  -0.02255076542,  0.01704398729,  0.07050706446,   0.1272066534,   0.1745604277,\
   0.2014744878,   0.2014744878,   0.1745604277,   0.1272066534,  0.07050706446,\
   0.01704398729, -0.02255076542, -0.04230293259, -0.04209024832, -0.02704929188,\
  -0.005489115138,  0.01393784769,  0.02487257868,  0.02488196269,  0.01566235721,\
   0.001884402242, -0.01080497075,  -0.0178749226, -0.01737787947, -0.01035293471,\
  -0.0001267225889, 0.009132681414,  0.01402795315,  0.01311222371, 0.007250499446,\
  -0.0009047139902,-0.008054497652, -0.01154625975, -0.01032224204,-0.005200890824,\
   0.001571267494, 0.007273038849,   0.0097791804, 0.008330924436, 0.003739544423,\
  -0.002024828456,-0.006658871658,-0.008433135226,-0.006822787691,-0.002643338637,\
   0.002340652747,   0.0061470652, 0.007356632967, 0.005631223787, 0.001791732735,\
  -0.002560514258,-0.005701822229,-0.006463991944, -0.00466015283,-0.001113970415,\
   0.002709657419, 0.005302053411, 0.005703321658, 0.003850434208,0.0005657746806,\
  -0.002804262796,-0.004934640136,-0.005041435361,-0.003163688118,-0.0001178953898,\
   0.002855427563, 0.004591232166, 0.004456216935, 0.002573836362,-0.0002499786206,\
  -0.002871319419,-0.004266605712,-0.003932794556,-0.002062810585,0.0005521377316,\
   0.002857826184, 0.003957144916,  0.00346039352, 0.001617376809,-0.0007993766922,\
  -0.00281974976,-0.003660683753,-0.003031382803,-0.001227727043, 0.001000010176,\
   0.002761132317, 0.003376020817, 0.002640358405, 0.000886691967,-0.001160169952,\
  -0.002684962004, -0.00310203596,-0.002282862319,-0.0005881907418, 0.001285271137,\
   0.002594103571, 0.002838404151, 0.001955616521,0.0003274458868,-0.001379889785,\
  -0.002491400577,-0.002585405251, -0.00165657443,-0.0001011107233, 0.001447157119,\
   0.002378498437, 0.002342519118,  0.00138323626,-9.448102355e-005,-0.001490679104,\
  -0.002257819287,-0.002110183705,-0.001134157064,0.0002619158186, 0.001513223164,\
   0.002131114481, 0.001888709841,0.0009079848533,-0.0004036253085,-0.001517228317,\
  -0.002000011271,-0.001678185421,-0.0007033856236,0.0005218694569, 0.001505041961,\
   0.001866118633, 0.001478948514,0.0005193172838,-0.0006186420214,-0.001478790655,\
  -0.001730839838,-0.001291149412,-0.0003546522348,0.0006958136219, 0.001440615626,\
   0.001595652779, 0.001115191495,0.0002086951135,-0.0007549041766,-0.001391842263,\
  -0.001461334876,-0.0009507808718,-8.005859854e-005,0.0007979645161, 0.001334693399,\
   0.001329447026,0.0007984013646,-3.193271186e-005,-0.0008263729396,-0.001270573004,\
  -0.001200831728,-0.0006580534973,0.0001281714503,0.0008415097836, 0.001200915431,\
   0.001076267334,0.0005294296425,-0.0002096275712,-0.0008449512534,-0.001127045951,\
  -0.0009565392975,-0.0004125079431,0.0002774739987,0.0008382110973, 0.001050422317,\
   0.0008423305699,0.0003070131061,-0.0003326688602,-0.0008226116188,-0.0009722104296,\
  -0.0007343280013,-0.0002129065397,0.0003759867686,0.0007992550964,0.0008932650671,\
   0.0006328151794,0.0001296115515,-0.0004082531668,-0.0007692913059,-0.0008145574247,\
  -0.0005378419883,-5.656403664e-005,0.0004310507502,0.0007341686287,0.0007371228421,\
   0.0004500737414,-6.337595551e-006,-0.0004447723913,-0.0006946527865,-0.000661567552,\
  -0.0003695303749,5.979820708e-005,0.0004506692349,0.0006519903545,0.0005886628642,\
   0.0002965501626,-0.0001041156211,-0.0004493572924,-0.0006068443181,-0.0005189483054,\
  -0.000230337726,0.0001401899353,0.0004419854668,0.0005602184101,0.0004527117708,\
   0.0001713849924,-0.0001687666081,-0.0004297963169,-0.0005134353996,-0.0003910067317,\
  -0.000119867349,0.0001897966868,0.0004126331769,0.0004658395774,0.0003325725556,\
   7.397215086e-005,-0.0002059252147,-0.0003939556773,-0.0004207049788,-0.0002811307495,\
  -3.707793076e-005,0.0002126409963,0.0003679615038,0.0003727058938,0.0002289074473,\
   2.793246949e-007,-0.0002217289875,-0.0003484986664,-0.0003356365487,-0.0001922601077,\
   1.823118509e-005, 0.000211613733, 0.000310628966,0.0002811293816,0.0001380053436,\
  -5.725489245e-005,-0.0002275148436,-0.0003065310011,-0.0002677424927,-0.0001344568736,\
   3.767904491e-005,0.0001801131148,0.0002319578343, 0.000179249153,4.165977953e-005,\
  -0.0001221083512,-0.000252182188,-0.0002992405789,-0.0002547842741,-0.0001417729945,\
  -1.486802466e-005,7.27102597e-005,7.971248851e-005,2.40697068e-006,-0.000135702925,\
  -0.0002846919233,-0.000398971024,-0.0004455255694,-0.0004262450675,-0.0003662098607,\
  -0.0003126577358, 0.001536677242}

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of _FPU_FILTER_H_

// End of File

