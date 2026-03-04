#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <optional>
#include <cstdint>


namespace HookConfig {

    constexpr const char* CONFIG_PATH = "hooks.json";

    struct HookEntry {
        uint32_t    pid = 0;
        std::string hookName;
        uintptr_t   dataRemote = 0;
        uintptr_t   shellcodeRemote = 0;
        uintptr_t   targetFunction = 0;
        uintptr_t   callSiteAddr = 0; // non-zero for call-site hooks (not vtable hooks)
    };

    // -----------------------------------------------------------------------
    // Internal helpers
    // -----------------------------------------------------------------------
    namespace detail {

        inline std::string uintToHex(uintptr_t v) {
            char buf[32];
            snprintf(buf, sizeof(buf), "0x%llX", (unsigned long long)v);
            return buf;
        }

        inline uintptr_t hexToUint(const std::string& s) {
            if (s.empty()) return 0;
            return (uintptr_t)strtoull(s.c_str(), nullptr, 16);
        }

        inline size_t findValueStart(const std::string& json, const std::string& key) {
            const std::string token = "\"" + key + "\":";
            size_t pos = 0;
            while (pos < json.size()) {
                auto hit = json.find(token, pos);
                if (hit == std::string::npos) return std::string::npos;

                if (hit > 0) {
                    char before = json[hit - 1];
                    if (before != ' ' && before != '\t' && before != '\n' &&
                        before != '\r' && before != '{' && before != ',') {
                        pos = hit + 1;
                        continue;
                    }
                }
                return hit + token.size();
            }
            return std::string::npos;
        }

        inline std::string extractString(const std::string& json, const std::string& key) {
            size_t after_colon = findValueStart(json, key);
            if (after_colon == std::string::npos) return {};
            auto q1 = json.find('"', after_colon);
            if (q1 == std::string::npos) return {};
            auto q2 = json.find('"', q1 + 1);
            if (q2 == std::string::npos) return {};
            return json.substr(q1 + 1, q2 - q1 - 1);
        }

        inline uint32_t extractUint(const std::string& json, const std::string& key) {
            size_t i = findValueStart(json, key);
            if (i == std::string::npos) return 0;
            while (i < json.size() &&
                (json[i] == ' ' || json[i] == '\t' ||
                    json[i] == '\n' || json[i] == '\r'))
                ++i;
            return (uint32_t)strtoul(json.c_str() + i, nullptr, 10);
        }

        inline HookEntry parseEntry(const std::string& block) {
            HookEntry e;
            e.pid = extractUint(block, "pid");
            e.hookName = extractString(block, "hookName");
            e.dataRemote = hexToUint(extractString(block, "dataRemote"));
            e.shellcodeRemote = hexToUint(extractString(block, "shellcodeRemote"));
            e.targetFunction = hexToUint(extractString(block, "targetFunction"));
            e.callSiteAddr = hexToUint(extractString(block, "callSiteAddr"));
            return e;
        }

        inline std::string serializeEntry(const HookEntry& e) {
            std::string s = std::string("  {\n")
                + "    \"pid\": " + std::to_string(e.pid) + ",\n"
                + "    \"hookName\": \"" + e.hookName + "\",\n"
                + "    \"dataRemote\": \"" + uintToHex(e.dataRemote) + "\",\n"
                + "    \"shellcodeRemote\": \"" + uintToHex(e.shellcodeRemote) + "\",\n"
                + "    \"targetFunction\": \"" + uintToHex(e.targetFunction) + "\"";
            if (e.callSiteAddr)
                s += ",\n    \"callSiteAddr\": \"" + uintToHex(e.callSiteAddr) + "\"";
            s += "\n  }";
            return s;
        }

    }

    inline std::vector<HookEntry> Load() {
        std::vector<HookEntry> entries;
        std::ifstream f(CONFIG_PATH);
        if (!f.is_open()) return entries;

        std::string json((std::istreambuf_iterator<char>(f)),
            std::istreambuf_iterator<char>());

        size_t pos = 0;
        while (pos < json.size()) {
            auto start = json.find('{', pos);
            if (start == std::string::npos) break;
            auto end = json.find('}', start);
            if (end == std::string::npos) break;
            std::string block = json.substr(start, end - start + 1);
            HookEntry e = detail::parseEntry(block);
            if (!e.hookName.empty())
                entries.push_back(e);
            pos = end + 1;
        }
        return entries;
    }

    inline bool Save(const std::vector<HookEntry>& entries) {
        std::ofstream f(CONFIG_PATH, std::ios::trunc);
        if (!f.is_open()) return false;

        f << "[\n";
        for (size_t i = 0; i < entries.size(); ++i) {
            f << detail::serializeEntry(entries[i]);
            if (i + 1 < entries.size()) f << ',';
            f << '\n';
        }
        f << "]\n";
        return f.good();
    }

    inline std::optional<HookEntry> Find(const std::string& hookName, uint32_t pid = 0) {
        for (auto& e : Load()) {
            if (e.hookName == hookName && (pid == 0 || e.pid == pid))
                return e;
        }
        return std::nullopt;
    }

    inline bool Upsert(const HookEntry& entry) {
        auto entries = Load();
        for (auto& e : entries) {
            if (e.hookName == entry.hookName) {
                e = entry;
                return Save(entries);
            }
        }
        entries.push_back(entry);
        return Save(entries);
    }

    inline bool Remove(const std::string& hookName) {
        auto entries = Load();
        auto before = entries.size();
        entries.erase(
            std::remove_if(entries.begin(), entries.end(),
                [&](const HookEntry& e) { return e.hookName == hookName; }),
            entries.end());
        if (entries.size() == before) return true; 
        return Save(entries);
    }

}