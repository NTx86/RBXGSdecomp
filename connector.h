#pragma once
#include "kernelindex.h"

/*class RBX::Connector : public RBX::KernelIndex { // Size=0x8
  //0x0004: fields for RBX::KernelIndex
  
  private: int32_t& getKernelIndex();
  public: void Connector(const RBX::Connector&);
  public: void Connector();
  public: virtual ~Connector();
  public: virtual void computeForce(const float, bool);
  public: virtual bool canThrottle() const;
  public: virtual bool getBroken();
  public: virtual float potentialEnergy();
  public: RBX::Connector& operator=(const RBX::Connector&);
  public: void __local_vftable_ctor_closure();
  public: virtual void* __vecDelDtor(uint32_t);
};/*/

namespace RBX {
	class Connector : public RBX::KernelIndex
	{
		public:
			__declspec(noinline) int& getKernelIndex() {return kernelIndex;}
			virtual ~Connector() {}
			virtual void computeForce(const float, bool) {}
			virtual bool canThrottle();
			virtual bool getBroken() {return false;} //placeholder
			virtual float potentialEnergy() {return 0;};
	};
}