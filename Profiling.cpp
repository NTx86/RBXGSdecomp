#include "Profiling.h"
#include <g3d/system.h>
using namespace RBX;

unsigned long RBX::Profiling::Mark::markTlsIndex;

Profiling::Profiler::Profiler(const char *name) : bucketTimeSpan(1.0)
{
  RBX::Profiling::Bucket *buckets; // eax

  //this->bucketTimeSpan = 1.0;
  this->currentBucket = 0;
  buckets = this->buckets;
  for (int i = 4095; i >= 0; --i )
  {
    buckets->sampleTimeSpan = 0.0;
    buckets->kernTimeSpan = 0;
    buckets->userTimeSpan = 0;
    buckets->frames = 0;
    ++buckets;
  }
  this->lastSampleTime = G3D::System::getTick();
  this->name = name;
}

Profiling::CodeProfiler::CodeProfiler(const char *name) : Profiler(name)
{
	parent = 0;
}

void Profiling::CodeProfiler::Log(__int64 kern, __int64 user, bool frameTick)
{
	this->lastSampleTime = G3D::System::getTick(); //PLACEHOLDER!
}