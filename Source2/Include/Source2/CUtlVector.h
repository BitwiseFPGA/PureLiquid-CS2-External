#pragma once
#include <GlobalData/Include.h>
#include <cstdint>
#include <cstddef>
#include <functional>

namespace Source2 {

	template <typename T = void>
	class CUtlVector {
	private:
		std::uint32_t m_iSize;
		char _pad[0x4];
		T* m_pData;
		void* pad;
	public:
		static CUtlVector<T>* FromAddr(uintptr_t pAddr, bool bRead = false) {
			return reinterpret_cast<CUtlVector<T>*>(bRead ? ::proc.ReadDirect<uintptr_t>(pAddr) : pAddr);
		}

		std::uint32_t GetSize() {
			return ::Globals::proc.ReadDirect<std::uint32_t>(reinterpret_cast<uintptr_t>(this) + offsetof(CUtlVector, m_iSize));
		}

		std::vector<T> GetData() {
			auto size = GetSize();
			return ::Globals::proc.ReadArray<T>(::Globals::proc.ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + offsetof(CUtlVector, m_pData)), size);
		}


		T* GetEntryAtIndex(int idx) {
			return ::Globals::proc.ReadDirect<T*>(::Globals::proc.ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + offsetof(CUtlVector, m_pData)) + (sizeof(T) * idx));
		}

		std::vector<uintptr_t> GetPtrList() {
			auto size = GetSize();
			if (size <= 0 || size >= 1000)
				return {};
			printf("CUtlVector<%s>->Size: %i\n", typeid(T).name(), size);
			auto pAddr = ::Globals::proc.ReadDirect<uintptr_t>(reinterpret_cast<uintptr_t>(this) + offsetof(CUtlVector, m_pData));
			std::vector<uintptr_t> vecPtrList{};
			for (int i = 0; i < size; i++) {
				vecPtrList.push_back(pAddr + (sizeof(T) * i));
			}
			return vecPtrList;
		}

		template <typename T2 = T, typename Func>
		std::vector<T2> MapPtrEntries(Func&& mappingFn) {
			auto size = GetSize();
			auto pAddr = ::Globals::proc.ReadDirect<uintptr_t>(
				reinterpret_cast<uintptr_t>(this) + offsetof(CUtlVector, m_pData));

			std::vector<T2> vecPtrList;
			vecPtrList.reserve(size);

			for (int i = 0; i < size; i++) {
				vecPtrList.push_back(mappingFn(pAddr + (sizeof(T) * i)));
			}
			return vecPtrList;
		}

	};

	static_assert(sizeof(CUtlVector<>) == 0x18);
}