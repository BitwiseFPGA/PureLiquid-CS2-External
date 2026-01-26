#pragma once

#include <GlobalData/Include.h>
#include <Source2/CUtlVector.h>

using namespace Globals;

namespace CS2 {

	enum class ETypeCategory : std::uint8_t {
		Schema_Builtin = 0,
		Schema_Ptr,
		Schema_Bitfield,
		Schema_FixedArray,
		Schema_Atomic,
		Schema_DeclaredClass,
		Schema_DeclaredEnum,
		Schema_None
	};


	class CSchemaSystemTypeScope;

	class CSchemaField {
	private:
		char sizePad[0x20];
	public:
		CACHED_PROPERTY_STRING_PTR(m_szName, 0x0, 256);
		CACHED_PROPERTY(uOffset, uint32_t, 0x10);
	};

	class CSchemaClass {
	private:
		inline static std::map<CSchemaClass*, std::map<int, CSchemaField*>> m_pFieldCache = { {} };
	public:

		CACHED_PROPERTY_STRING_PTR(m_szName, 0x8, 256);
		CACHED_PROPERTY_STRING_PTR(m_szModuleName, 0x10, 256);
		CACHED_PROPERTY(m_nNumFields, uint16_t, 0x1C);
		CACHED_PROPERTY(m_pFieldsPtr, uintptr_t, 0x28);

		CSchemaField* GetField(int idx) {
			if (idx < 0 || idx > m_nNumFields)
				return nullptr;

			if (!m_pFieldCache.contains(this)) {
				m_pFieldCache[this] = {};
			}
			if (m_pFieldCache[this].contains(idx)) {
				return m_pFieldCache[this][idx];
			}
			auto pFieldAddr = m_pFieldsPtr + (sizeof(CSchemaField) * (idx));
			return m_pFieldCache[this][idx] = reinterpret_cast<CSchemaField*>(pFieldAddr);
		}
	};


	class CSchemaDeclaredClass {
	public:
		CACHED_PROPERTY_STRING_PTR(m_szName, 0x8, 256);
		CACHED_PROPERTY(pGlobalScope, CSchemaSystemTypeScope*, 0x10);
		CACHED_PROPERTY(eType, ETypeCategory, 0x18);
		CACHED_PROPERTY(pSchemaClass, CSchemaClass*, 0x20);

	};
	
	class CSchemaDeclaredClassEntry {
	
	private:
		char sizePad[0x18];
	
	public:
		PROPERTY(m_pDeclaredClass, CSchemaDeclaredClass*, 0x10);
	
	};

	class CSchemaSystemTypeScope {
	private:
		inline static std::map<CSchemaSystemTypeScope*, std::map<int, CSchemaDeclaredClass*>> m_pClassCache = { {} };
	public:
		CACHED_PROPERTY_STRING(m_szName, 0x8, 256);
		
		CACHED_PROPERTY(pGlobalScope, CSchemaSystemTypeScope*, 0x108);
		
		CACHED_PROPERTY(m_nNumDeclaredClasses, uint16_t, 0x470);
		CACHED_PROPERTY(m_pDeclaredClassesPtr, uintptr_t, 0x478);


		CSchemaDeclaredClass* GetDeclaredClass(int idx) {
			if (idx < 0 || idx > m_nNumDeclaredClasses)
				return nullptr;

			if (!m_pClassCache.contains(this)) {
				m_pClassCache[this] = {};
			}
			if (m_pClassCache[this].contains(idx)) {
				return m_pClassCache[this][idx];
			}
			auto pClassAddr = m_pDeclaredClassesPtr + (sizeof(CSchemaDeclaredClassEntry) * (idx));
			auto pClass =  reinterpret_cast<CSchemaDeclaredClassEntry*>(pClassAddr);
			return m_pClassCache[this][idx] = pClass->m_pDeclaredClass;
		}

	};


	class CSchemaSystem {
	private:
		
		inline static std::vector< CSchemaSystemTypeScope*> m_pScopeArrayCached = {};
		inline static std::map<std::string, uint32_t> m_OffsetMap = {};

		NESTED_PROPERTY(m_pScopeArray, ::Source2::CUtlVector<CSchemaSystemTypeScope**>, 0x190);

	public:
		
		uint32_t GetOffset(std::string key) {
			auto it = m_OffsetMap.find(key);
			if (it != m_OffsetMap.end()) {
				return it->second;
			}

			auto pScopes = GetAllScopes();

			size_t firstArrow = key.find("->");
			size_t secondArrow = key.find("->", firstArrow + 2);

			if (firstArrow == std::string::npos || secondArrow == std::string::npos) {
				throw std::runtime_error("Invalid key format! Expected: mod->class->field");
			}

			std::string szMod = key.substr(0, firstArrow);
			std::string szClass= key.substr(firstArrow + 2, secondArrow - firstArrow - 2);
			std::string szField= key.substr(secondArrow + 2);

			for (const auto& scope : pScopes) {
				std::string scopeName = scope->m_szName;
				if (scopeName != szMod)
					continue;

				if (scope->m_nNumDeclaredClasses <= 0) {
					throw std::runtime_error(szMod + " doesn't have any classes!!");
				}

				for (int i = 0; i < scope->m_nNumDeclaredClasses; i++) {
					auto pClass = scope->GetDeclaredClass(i);
					if (!pClass)
						continue;

					std::string className = pClass->m_szName;
					if (className != szClass)
						continue;

					auto pClassData = pClass->pSchemaClass;
					if (!pClassData) {
						throw std::runtime_error(szClass + " doesn't have any class data!!");
					}

					if (pClassData->m_nNumFields <= 0) {
						throw std::runtime_error(szClass + " doesn't have any fields!!");
					}

					for (int j = 0; j < pClassData->m_nNumFields; j++) {
						auto pField = pClassData->GetField(j);
						if (!pField)
							continue;

						std::string fieldName = pField->m_szName;
						if (fieldName == szField) {
							return m_OffsetMap[key] = pField->uOffset;
						}
					}
				}
			}

			throw std::runtime_error(key + " not found in schema system!!");
		}
		uint32_t GetOffset(std::string szMod, std::string szClass, std::string szField) {
			std::string key = szMod + "->" + szClass + "->" + szField;
			return GetOffset(key);
		}

		std::vector<CSchemaSystemTypeScope*> GetAllScopes() {
			if (m_pScopeArrayCached.empty()) {
				auto pScopes = m_pScopeArray;
				m_pScopeArrayCached = pScopes->MapPtrEntries<CSchemaSystemTypeScope*>([](uintptr_t addr) -> CSchemaSystemTypeScope* {
					return reinterpret_cast<CSchemaSystemTypeScope*>(proc.ReadDirect<uintptr_t>(addr));
					});
			}
			return m_pScopeArrayCached;
		}
	};

}