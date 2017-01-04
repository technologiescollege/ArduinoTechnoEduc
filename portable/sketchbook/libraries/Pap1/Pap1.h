 #ifndef Pap1_h
    #define Pap1_h
 
#define SINGLE_STEPPING 0
#define FULL_STEPPING 1
#define HALF_STEPPING 2
#define AUTO_RELAX 0
#define NO_AUTO_RELAX 1
#define IDLE 0
#define CC 1
#define RELATIVE 2
#define ABSOLUTE 3
#define CCW 0
#define CW 1
        
    //#include "WProgram.h"
        
    class Pap1
    {
      public:
        Pap1(int add);
        void Begin(int stepping_mode,int relax);
        void SetMode(int mde);
        void Set_Registre(int reg,int data);
        void SpeedA(int speed);
        void SpeedB(int speed);
        void DirA(int dir);
        void DirB(int dir);
        void StepA(int step);
        void StepB(int step);
        void OrigineA();
        void OrigineB();
        void TargetA(long target);
        void TargetB(long target);
        bool Busy();               
      private:
        int _add_device;
    };
        
#endif
