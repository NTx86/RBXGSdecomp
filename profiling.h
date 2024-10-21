#pragma once
#include <windows.h>
#include <boost/noncopyable.hpp>
#include <string>

namespace RBX
{
	namespace Profiling
	{
		class Bucket
		{
			public:
				long double sampleTimeSpan;
				__int64 kernTimeSpan;
				__int64 userTimeSpan;
				int frames;
		};

		class Profiler : boost::noncopyable_::noncopyable
		{
			public:
				const long double bucketTimeSpan;
				int currentBucket;
				RBX::Profiling::Bucket buckets[4096];
				long double lastSampleTime;
				std::string name;
				Profiler::Profiler(const char *name);
		};



		class CodeProfiler : RBX::Profiling::Profiler
		{
			public:
				Profiling::CodeProfiler *parent;
				CodeProfiler::CodeProfiler(const char *name);
				void Profiling::CodeProfiler::Log(__int64 kern, __int64 user, bool frameTick);
		};

		class Mark
		{
			private:
				Profiling::CodeProfiler& section;
				Profiling::CodeProfiler* enclosingSection;
				FILETIME creationTime;
				FILETIME exitTime;
				FILETIME kernelTime;
				FILETIME userTime;
				bool frameTick;
			public: 
				static unsigned long markTlsIndex;
				Mark(Profiling::CodeProfiler& sectionSet, bool frameTickSet) : section(sectionSet), frameTick(frameTickSet)
				{
					if ( RBX::Profiling::Mark::markTlsIndex )
					{
						enclosingSection = (Profiling::CodeProfiler*)TlsGetValue(RBX::Profiling::Mark::markTlsIndex);
						GetThreadTimes(GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime);
						TlsSetValue(RBX::Profiling::Mark::markTlsIndex, (void*)&sectionSet);
					}
				}

				~Mark(void)
				{
					FILETIME currUserTime;
					FILETIME currKernelTime;
					if ( RBX::Profiling::Mark::markTlsIndex )
					{
						ULARGE_INTEGER currKernelTime64, currUserTime64, kernelTime64, userTime64;
						__int64 kernelTimeDelta, userTimeDelta;

						TlsSetValue(RBX::Profiling::Mark::markTlsIndex, enclosingSection);
						
						GetThreadTimes(GetCurrentThread(), &creationTime, &exitTime, &currKernelTime, &currUserTime);

						kernelTime64.LowPart = kernelTime.dwLowDateTime;
						kernelTime64.HighPart = kernelTime.dwHighDateTime;
						userTime64.LowPart = userTime.dwLowDateTime;
						userTime64.HighPart = userTime.dwHighDateTime;

						currKernelTime64.LowPart = currKernelTime.dwLowDateTime;
						currKernelTime64.HighPart = currKernelTime.dwHighDateTime;
						currUserTime64.LowPart = currUserTime.dwLowDateTime;
						currUserTime64.HighPart = currUserTime.dwHighDateTime;

						kernelTimeDelta = (currKernelTime64.QuadPart - kernelTime64.QuadPart);
						userTimeDelta = (currUserTime64.QuadPart - userTime64.QuadPart);

						section.Log(kernelTimeDelta, userTimeDelta, frameTick);

						if (enclosingSection && enclosingSection != &section && enclosingSection != section.parent)
						{
							enclosingSection->Log(-kernelTimeDelta, -userTimeDelta, false);
						}
					}
				}
		};
	}
}