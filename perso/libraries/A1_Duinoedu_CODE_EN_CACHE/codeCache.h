//******************************************************************//
//	                                                                //
//	                  CODE CACHE POUR ARDUBLOCK                     //
//	    PLACEZ ICI LES SECTIONS DE CODE QUE VOUS VOULEZ APPELER     // 
//	            PAR UN MOT CLE DANS ARDUINO OU ARDUBLOCK            //
//                                                                  //
//******************************************************************//
/*
	Par David Souder - souder.d@gmail.com
	Dernière modification : le 31/03/2015
*/

/* UTILITE :
	- Permet de créer des blocs de code personnels et cachés pouvant être appelés 
	par un simple mot clé dans Arduino ou Ardublock.
*/

/* PRINCIPE D'UTILISATION :
	- Il faut considérer 3 zones :
		o HEAD (au dessus du setup)
		o SETUP
		o LOOP
	- Suivant la zone dans laquelle vous voulez placer du code personnel caché,
	écrivez celui-ci dans la zone : HEAD, SETUP ou LOOP en suivant ce canvas.
		
*/	

//-- A GARDER SVP 
#ifndef codeCache_H
#define codeCache_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif


//[ ZONE HEAD ]//////////////////////////////////////////////
	// 01 : Ecrivez le code que vous voulez ajouter en entête ci dessous (pas d'erreur de syntaxe SVP):	
	#define HEAD_EN_CACHE0 					    	    \
		int maVariableEnCache;							\
		/*A remplacer par vos déclarations*/			\
		/*A remplacer par vos déclarations*/			\
		// Espace à garder !
	#define HEAD_EN_CACHE1 					    	    \
		/*A remplacer par vos déclarations*/			\
		/*A remplacer par vos déclarations*/			\
		/*A remplacer par vos déclarations*/			\
		// Espace à garder !
	#define HEAD_EN_CACHE2 					    	    \
		/*A remplacer par vos déclarations*/			\
		/*A remplacer par vos déclarations*/			\
		/*A remplacer par vos déclarations*/			\
		// Espace à garder !
		
	// 02 Utilisez l'étiquette AJOUT_HEAD dans Ardublock pour appeler cette section de programme
	// 03 ou écrivez HEAD_EN_CACHE0,1,2... ; au dessus du setup() du logiciel Arduino si vous n'utilisez pas Ardublock		
		/* Exemples :
		int maVariableCachee=0;			// Une variable integer 
		String maChaineCachee;			// Une chaîne de caractères
		char monTableauCache[10];		// Un tableau
		*/
	
	
	
	
	
//[ ZONE SETUP ]//////////////////////////////////////////////
	// 01 : Ecrivez le code que vous voulez ajouter dans le setup() ci dessous :													
	#define SETUP_EN_CACHE0 do					    	\
	{													\
		maVariableEnCache = 50; 						\
		/*A remplacer par votre code*/ 					\
		/*A remplacer par votre code*/ 					\
	} while(0)
	
	#define SETUP_EN_CACHE1 do					        \
	{													\
		/*A remplacer par votre code*/ 					\
		/*A remplacer par votre code*/ 					\
		/*A remplacer par votre code*/ 					\
	} while(0)
	
	#define SETUP_EN_CACHE2 do					        \
	{													\
		/*A remplacer par votre code*/ 					\
		/*A remplacer par votre code*/ 					\
		/*A remplacer par votre code*/ 					\
	} while(0)
	
	
	// 02 Utilisez l'étiquette SECTION_SETUP dans Ardublock pour appeler cette section de programme
	// 03 ou écrivez SETUP_EN_CACHE0,1,2... ; dans le setup() du logiciel Arduino si vous n'utilisez pas Ardublock
		/* Exemples :
		maVariableCachee=50;			
		maChaineCachee="Hello World !";			
		monTableauCache[0]='a';	
		monTableauCache[1]='b';	
		*/
	
	
	
	
//[ ZONE LOOP ]////////////////////////////////////////////// 
	// 01 : Ecrivez le code que vous voulez ajouter dans le loop() ci dessous :													
	#define LOOP_EN_CACHE0 do					    	\
	{													\
		/*A remplacer par votre code*/					\
		/*A remplacer par votre code*/					\
		/*A remplacer par votre code*/					\
	} while(0)
	
	#define LOOP_EN_MEMOIRE1 do					    	\
	{													\
		/*A remplacer par votre code*/					\
		/*A remplacer par votre code*/ 					\
		/*A remplacer par votre code*/ 					\
	} while(0)
	
	#define LOOP_EN_MEMOIRE2 do			    	        \
	{													\
		/*A remplacer par votre code*/					\
		/*A remplacer par votre code*/ 					\
		/*A remplacer par votre code*/ 					\
	} while(0)
	
	// 02 Utilisez l'étiquette AJOUT_LOOP dans Ardublock pour appeler cette section de programme
	// 03 ou écrivez LOOP_EN_CACHE0,1,2... ; dans le setup() du logiciel Arduino si vous n'utilisez pas Arduino
		/* Exemples :
		maVariableCachee++;					// Incrément de 1 à chaque passage dans le loop()		
		*/

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
// Plus tard, peut-être ferrez-vous vos propres libraries
class codeCache{

	public:
	
	codeCache();

	protected:



};
#endif
