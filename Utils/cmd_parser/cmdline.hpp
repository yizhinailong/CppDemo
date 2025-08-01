#pragma once

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cxxabi.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

namespace cmdline {
    namespace detail {
        template <typename Target, typename Source, bool Same>
        class lexical_cast_t {
        public:
            static Target cast(const Source& arg) {
                Target ret;
                std::stringstream ss;
                if (!(ss << arg && ss >> ret && ss.eof())) {
                    throw std::bad_cast();
                }

                return ret;
            }
        };

        template <typename Target, typename Source>
        class lexical_cast_t<Target, Source, true> {
        public:
            static Target cast(const Source& arg) {
                return arg;
            }
        };

        template <typename Source>
        class lexical_cast_t<std::string, Source, false> {
        public:
            static std::string cast(const Source& arg) {
                std::ostringstream ss;
                ss << arg;
                return ss.str();
            }
        };

        template <typename Target>
        class lexical_cast_t<Target, std::string, false> {
        public:
            static Target cast(const std::string& arg) {
                Target ret;
                std::istringstream ss(arg);
                if (!(ss >> ret && ss.eof())) {
                    throw std::bad_cast();
                }
                return ret;
            }
        };

        template <typename T1, typename T2>
        struct is_same {
            static const bool value = false;
        };

        template <typename T>
        struct is_same<T, T> {
            static const bool value = true;
        };

        template <typename Target, typename Source>
        Target lexical_cast(const Source& arg) {
            return lexical_cast_t<Target, Source, detail::is_same<Target, Source>::value>::cast(arg);
        }

        static inline std::string demangle(const std::string& name) {
            int status = 0;
            char* p = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
            std::string ret(p);
            free(p);
            return ret;
        }

        template <class T>
        std::string readable_typename() {
            return demangle(typeid(T).name());
        }

        template <class T>
        std::string default_value(T def) {
            return detail::lexical_cast<std::string>(def);
        }

        template <>
        inline std::string readable_typename<std::string>() {
            return "string";
        }

    } // namespace detail

    //-----

    class cmdline_error : public std::exception {
    public:
        cmdline_error(const std::string& msg)
            : m_msg(msg) {}

        ~cmdline_error() throw() {}

        const char* what() const throw() {
            return m_msg.c_str();
        }

    private:
        std::string m_msg;
    };

    template <class T>
    struct default_reader {
        T operator()(const std::string& str) {
            return detail::lexical_cast<T>(str);
        }
    };

    template <class T>
    struct range_reader {
        range_reader(const T& low, const T& high)
            : low(low), high(high) {}

        T operator()(const std::string& s) const {
            T ret = default_reader<T>()(s);
            if (!(ret >= low && ret <= high)) {
                throw cmdline::cmdline_error("range_error");
            }
            return ret;
        }

    private:
        T low, high;
    };

    template <class T>
    range_reader<T> range(const T& low, const T& high) {
        return range_reader<T>(low, high);
    }

    template <class T>
    struct oneof_reader {
        T operator()(const std::string& s) {
            T ret = default_reader<T>()(s);
            if (std::find(alt.begin(), alt.end(), ret) == alt.end()) {
                throw cmdline_error("");
            }
            return ret;
        }

        void add(const T& v) {
            alt.push_back(v);
        }

    private:
        std::vector<T> alt;
    };

    template <class T>
    oneof_reader<T> oneof(T a1) {
        oneof_reader<T> ret;
        ret.add(a1);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2, T a3) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        ret.add(a3);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2, T a3, T a4) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        ret.add(a3);
        ret.add(a4);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        ret.add(a3);
        ret.add(a4);
        ret.add(a5);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        ret.add(a3);
        ret.add(a4);
        ret.add(a5);
        ret.add(a6);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        ret.add(a3);
        ret.add(a4);
        ret.add(a5);
        ret.add(a6);
        ret.add(a7);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        ret.add(a3);
        ret.add(a4);
        ret.add(a5);
        ret.add(a6);
        ret.add(a7);
        ret.add(a8);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        ret.add(a3);
        ret.add(a4);
        ret.add(a5);
        ret.add(a6);
        ret.add(a7);
        ret.add(a8);
        ret.add(a9);
        return ret;
    }

    template <class T>
    oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9, T a10) {
        oneof_reader<T> ret;
        ret.add(a1);
        ret.add(a2);
        ret.add(a3);
        ret.add(a4);
        ret.add(a5);
        ret.add(a6);
        ret.add(a7);
        ret.add(a8);
        ret.add(a9);
        ret.add(a10);
        return ret;
    }

    //-----

    class parser {
    public:
        parser() {
        }

        ~parser() {
            for (std::map<std::string, option_base*>::iterator p = m_options.begin();
                 p != m_options.end();
                 p++) {
                delete p->second;
            }
        }

        void add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "") {
            if (m_options.count(name)) {
                throw cmdline_error("multiple definition: " + name);
            }
            m_options[name] = new option_without_value(name, short_name, desc);
            m_ordered.push_back(m_options[name]);
        }

        template <class T>
        void add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "",
                 bool need = true,
                 const T def = T()) {
            add(name, short_name, desc, need, def, default_reader<T>());
        }

        template <class T, class F>
        void add(const std::string& name,
                 char short_name = 0,
                 const std::string& desc = "",
                 bool need = true,
                 const T def = T(),
                 F reader = F()) {
            if (m_options.count(name)) {
                throw cmdline_error("multiple definition: " + name);
            }
            m_options[name] = new option_with_value_with_reader<T, F>(name, short_name, need, def, desc, reader);
            m_ordered.push_back(m_options[name]);
        }

        void footer(const std::string& f) {
            m_ftr = f;
        }

        void set_program_name(const std::string& name) {
            m_prog_name = name;
        }

        bool exist(const std::string& name) const {
            if (m_options.count(name) == 0) {
                throw cmdline_error("there is no flag: --" + name);
            }
            return m_options.find(name)->second->has_set();
        }

        template <class T>
        const T& get(const std::string& name) const {
            if (m_options.count(name) == 0) {
                throw cmdline_error("there is no flag: --" + name);
            }
            const option_with_value<T>* p = dynamic_cast<const option_with_value<T>*>(m_options.find(name)->second);
            if (p == NULL) {
                throw cmdline_error("type mismatch flag '" + name + "'");
            }
            return p->get();
        }

        const std::vector<std::string>& rest() const {
            return m_others;
        }

        bool parse(const std::string& arg) {
            std::vector<std::string> args;

            std::string buf;
            bool in_quote = false;
            for (std::string::size_type i = 0; i < arg.length(); i++) {
                if (arg[i] == '\"') {
                    in_quote = !in_quote;
                    continue;
                }

                if (arg[i] == ' ' && !in_quote) {
                    args.push_back(buf);
                    buf = "";
                    continue;
                }

                if (arg[i] == '\\') {
                    i++;
                    if (i >= arg.length()) {
                        m_errors.push_back("unexpected occurrence of '\\' at end of string");
                        return false;
                    }
                }

                buf += arg[i];
            }

            if (in_quote) {
                m_errors.push_back("quote is not closed");
                return false;
            }

            if (buf.length() > 0) {
                args.push_back(buf);
            }

            for (size_t i = 0; i < args.size(); i++) {
                std::cout << "\"" << args[i] << "\"" << std::endl;
            }

            return parse(args);
        }

        bool parse(const std::vector<std::string>& args) {
            int argc = static_cast<int>(args.size());
            std::vector<const char*> argv(argc);

            for (int i = 0; i < argc; i++) {
                argv[i] = args[i].c_str();
            }

            return parse(argc, &argv[0]);
        }

        bool parse(int argc, const char* const argv[]) {
            m_errors.clear();
            m_others.clear();

            if (argc < 1) {
                m_errors.push_back("argument number must be longer than 0");
                return false;
            }
            if (m_prog_name == "") {
                m_prog_name = argv[0];
            }

            std::map<char, std::string> lookup;
            for (std::map<std::string, option_base*>::iterator p = m_options.begin();
                 p != m_options.end();
                 p++) {
                if (p->first.length() == 0) {
                    continue;
                }
                char initial = p->second->short_name();
                if (initial) {
                    if (lookup.count(initial) > 0) {
                        lookup[initial] = "";
                        m_errors.push_back(std::string("short option '") + initial + "' is ambiguous");
                        return false;
                    } else {
                        lookup[initial] = p->first;
                    }
                }
            }

            for (int i = 1; i < argc; i++) {
                if (strncmp(argv[i], "--", 2) == 0) {
                    const char* p = strchr(argv[i] + 2, '=');
                    if (p) {
                        std::string name(argv[i] + 2, p);
                        std::string val(p + 1);
                        set_option(name, val);
                    } else {
                        std::string name(argv[i] + 2);
                        if (m_options.count(name) == 0) {
                            m_errors.push_back("undefined option: --" + name);
                            continue;
                        }
                        if (m_options[name]->has_value()) {
                            if (i + 1 >= argc) {
                                m_errors.push_back("option needs value: --" + name);
                                continue;
                            } else {
                                i++;
                                set_option(name, argv[i]);
                            }
                        } else {
                            set_option(name);
                        }
                    }
                } else if (strncmp(argv[i], "-", 1) == 0) {
                    if (!argv[i][1]) {
                        continue;
                    }
                    char last = argv[i][1];
                    for (int j = 2; argv[i][j]; j++) {
                        last = argv[i][j];
                        if (lookup.count(argv[i][j - 1]) == 0) {
                            m_errors.push_back(std::string("undefined short option: -") + argv[i][j - 1]);
                            continue;
                        }
                        if (lookup[argv[i][j - 1]] == "") {
                            m_errors.push_back(std::string("ambiguous short option: -") + argv[i][j - 1]);
                            continue;
                        }
                        set_option(lookup[argv[i][j - 1]]);
                    }

                    if (lookup.count(last) == 0) {
                        m_errors.push_back(std::string("undefined short option: -") + last);
                        continue;
                    }
                    if (lookup[last] == "") {
                        m_errors.push_back(std::string("ambiguous short option: -") + last);
                        continue;
                    }

                    if (i + 1 < argc && m_options[lookup[last]]->has_value()) {
                        set_option(lookup[last], argv[i + 1]);
                        i++;
                    } else {
                        set_option(lookup[last]);
                    }
                } else {
                    m_others.push_back(argv[i]);
                }
            }

            for (std::map<std::string, option_base*>::iterator p = m_options.begin();
                 p != m_options.end();
                 p++) {
                if (!p->second->valid()) {
                    m_errors.push_back("need option: --" + std::string(p->first));
                }
            }

            return m_errors.size() == 0;
        }

        void parse_check(const std::string& arg) {
            if (!m_options.count("help")) {
                add("help", '?', "print this message");
            }
            check(0, parse(arg));
        }

        void parse_check(const std::vector<std::string>& args) {
            if (!m_options.count("help")) {
                add("help", '?', "print this message");
            }
            check(args.size(), parse(args));
        }

        void parse_check(int argc, char* argv[]) {
            if (!m_options.count("help")) {
                add("help", '?', "print this message");
            }
            check(argc, parse(argc, argv));
        }

        std::string error() const {
            return m_errors.size() > 0 ? m_errors[0] : "";
        }

        std::string error_full() const {
            std::ostringstream oss;
            for (size_t i = 0; i < m_errors.size(); i++) {
                oss << m_errors[i] << std::endl;
            }
            return oss.str();
        }

        std::string usage() const {
            std::ostringstream oss;
            oss << "usage: " << m_prog_name << " ";
            for (size_t i = 0; i < m_ordered.size(); i++) {
                if (m_ordered[i]->must()) {
                    oss << m_ordered[i]->short_description() << " ";
                }
            }

            oss << "[options] ... " << m_ftr << std::endl;
            oss << "options:" << std::endl;

            size_t max_width = 0;
            for (size_t i = 0; i < m_ordered.size(); i++) {
                max_width = std::max(max_width, m_ordered[i]->name().length());
            }
            for (size_t i = 0; i < m_ordered.size(); i++) {
                if (m_ordered[i]->short_name()) {
                    oss << "  -" << m_ordered[i]->short_name() << ", ";
                } else {
                    oss << "      ";
                }

                oss << "--" << m_ordered[i]->name();
                for (size_t j = m_ordered[i]->name().length(); j < max_width + 4; j++) {
                    oss << ' ';
                }
                oss << m_ordered[i]->description() << std::endl;
            }
            return oss.str();
        }

    private:
        void check(int argc, bool ok) {
            if ((argc == 1 && !ok) || exist("help")) {
                std::cerr << usage();
                exit(0);
            }

            if (!ok) {
                std::cerr << error() << std::endl
                          << usage();
                exit(1);
            }
        }

        void set_option(const std::string& name) {
            if (m_options.count(name) == 0) {
                m_errors.push_back("undefined option: --" + name);
                return;
            }
            if (!m_options[name]->set()) {
                m_errors.push_back("option needs value: --" + name);
                return;
            }
        }

        void set_option(const std::string& name, const std::string& value) {
            if (m_options.count(name) == 0) {
                m_errors.push_back("undefined option: --" + name);
                return;
            }
            if (!m_options[name]->set(value)) {
                m_errors.push_back("option value is invalid: --" + name + "=" + value);
                return;
            }
        }

        class option_base {
        public:
            virtual ~option_base() {}

            virtual bool has_value() const = 0;
            virtual bool set() = 0;
            virtual bool set(const std::string& value) = 0;
            virtual bool has_set() const = 0;
            virtual bool valid() const = 0;
            virtual bool must() const = 0;

            virtual const std::string& name() const = 0;
            virtual char short_name() const = 0;
            virtual const std::string& description() const = 0;
            virtual std::string short_description() const = 0;
        };

        class option_without_value : public option_base {
        public:
            option_without_value(const std::string& name,
                                 char short_name,
                                 const std::string& desc)
                : nam(name), snam(short_name), desc(desc), has(false) {
            }

            ~option_without_value() {}

            bool has_value() const {
                return false;
            }

            bool set() {
                has = true;
                return true;
            }

            bool set(const std::string&) {
                return false;
            }

            bool has_set() const {
                return has;
            }

            bool valid() const {
                return true;
            }

            bool must() const {
                return false;
            }

            const std::string& name() const {
                return nam;
            }

            char short_name() const {
                return snam;
            }

            const std::string& description() const {
                return desc;
            }

            std::string short_description() const {
                return "--" + nam;
            }

        private:
            std::string nam;
            char snam;
            std::string desc;
            bool has;
        };

        template <class T>
        class option_with_value : public option_base {
        public:
            option_with_value(const std::string& name,
                              char short_name,
                              bool need,
                              const T& def,
                              const std::string& desc)
                : nam(name), snam(short_name), need(need), has(false), def(def), actual(def) {
                this->desc = full_description(desc);
            }

            ~option_with_value() {}

            const T& get() const {
                return actual;
            }

            bool has_value() const {
                return true;
            }

            bool set() {
                return false;
            }

            bool set(const std::string& value) {
                try {
                    actual = read(value);
                    has = true;
                } catch (const std::exception& e) {
                    return false;
                }
                return true;
            }

            bool has_set() const {
                return has;
            }

            bool valid() const {
                if (need && !has) {
                    return false;
                }
                return true;
            }

            bool must() const {
                return need;
            }

            const std::string& name() const {
                return nam;
            }

            char short_name() const {
                return snam;
            }

            const std::string& description() const {
                return desc;
            }

            std::string short_description() const {
                return "--" + nam + "=" + detail::readable_typename<T>();
            }

        protected:
            std::string full_description(const std::string& desc) {
                return desc + " (" + detail::readable_typename<T>() +
                       (need ? "" : " [=" + detail::default_value<T>(def) + "]") + ")";
            }

            virtual T read(const std::string& s) = 0;

            std::string nam;
            char snam;
            bool need;
            std::string desc;

            bool has;
            T def;
            T actual;
        };

        template <class T, class F>
        class option_with_value_with_reader : public option_with_value<T> {
        public:
            option_with_value_with_reader(const std::string& name,
                                          char short_name,
                                          bool need,
                                          const T def,
                                          const std::string& desc,
                                          F reader)
                : option_with_value<T>(name, short_name, need, def, desc), reader(reader) {
            }

        private:
            T read(const std::string& s) {
                return reader(s);
            }

            F reader;
        };

        std::map<std::string, option_base*> m_options;
        std::vector<option_base*> m_ordered;
        std::string m_ftr;

        std::string m_prog_name;
        std::vector<std::string> m_others;

        std::vector<std::string> m_errors;
    };
} // namespace cmdline
