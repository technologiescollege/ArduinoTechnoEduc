//*************************************************************************
// EDU : Debug 
// David Souder - 07/2014
// www.duinoedu.com
//*************************************************************************

/*EDU US*/ // DEBUG !
//#define DEBUG   		// Uncoment if debug !
#ifdef DEBUG
	#define DEBUG(val) Serial.print(val)
	#define DEBUGLN(val) Serial.println(val)
	
	#define DEBUG2(val1,val2) do								\
		{														\
			Serial.print(val1);									\
			Serial.print(" ");									\
			Serial.println(val2);								\
		} while(0)
	
	#define DEBUG_TAB(nom,tab,val1,val2) do     				\
		{														\
			Serial.print("\n");                 				\
			Serial.println(nom);                				\
			Serial.print("Col:"); 		   						\
			Serial.print("["); 		    						\
			Serial.print("0"); 									\
			Serial.print("] : "); 		    					\
			for(int16_t i=val1;i<val2;i++){ 					\
				Serial.print(tab[i]); 							\
				Serial.print(" "); 		    					\
			}                                   				\
			Serial.print("\n");             					\
		}while(0)
		
		#define DEBUG_TAB2(nom,tab,val11,val12,val21,val22) do  \
		{														\
			Serial.println("");                 				\
			Serial.println(nom);                				\
			for(int16_t j=val21;j<=val22;j++){ 					\
				Serial.print("Col:"); 		   					\
				Serial.print("["); 		    					\
				Serial.print(j); 								\
				Serial.print("] : "); 		    				\
				for(int16_t i=val11;i<=val12;i++){ 				\
					Serial.print(tab[i][j]); 					\
					Serial.print(" "); 		    				\
				}												\
				Serial.print("\n");             				\
			}   												\
			Serial.print("\n");      							\
		}while(0)	
	
	
#else 
	#define DEBUG(val)
	#define DEBUGLN(val)
	#define DEBUG2(val1,val2)
	#define DEBUG_TAB(nom,tab,val1,val2)
	#define DEBUG_TAB2(nom,tab,val11,val12,val21,val22)
#endif
//*/