#include <vector>
#include <string>
#include <regex>
#include <map>

typedef std::string str_t;
typedef const char  key_c;

/*
  A minimal interface for parsing command line arguments
  like:

  `exec -i <value> -y -L<value> -s=<value> -z80`

  Note: unlike `getopt` this class parse all arguments without validations.
        use methods `has_option` and `get_value` for check/get passed options.
*/
class DaiOpts {
	std::regex m_rx;
	std::map<key_c, str_t> m_opt;
public:

	~DaiOpts(){}
/*
  A initialization require for safely call `DaiOpts::has_option(c)`.

  If user didn't pass any option, `has_option(c)` returns false,
           because option in map object conains an empty string

  If user pass non-value option (-h for example) his value will be "1".
*/
	 DaiOpts(std::vector<char> opt_keys)
	              : m_rx( str_t("^-([a-zA-Z])=?(.*)$") )
	{
		for (const auto c : opt_keys) {
			m_opt[c] = str_t("");
		}
	}
/*
  parse arguments from `int main()` for example

  Note: `argv[0]` will be skipped
*/
	void parse(int argc, const char *argv[])
	{
		int   i = 1;
		char  c = 0;
		str_t y = "1";

		for(; i < argc; i++) {

			str_t   arg = argv[i];
			uint32_t sz = arg.size();
			bool is_key = sz != 0 && arg.at(0) == '-';

			if ( is_key ) {
				if (sz == 2) m_opt[(c = arg.at(1))] = y; else
				if (sz  > 2) {
					std::smatch mc;
					if (std::regex_search(arg, mc, m_rx)) {
						key_c k  = mc[1].str().at(0);
						m_opt[k] = mc[2].str();
					}
					c = 0;
				}
			} else if (c && sz != 0) {
				m_opt[ c ] = arg;
			}
		}
	}

	auto has_option(key_c c) -> const bool {
		return !m_opt[c].empty();
	}
	auto get_value(key_c c) -> const str_t {
		return m_opt[c];
	}
	auto begin() -> const std::map<key_c, str_t>::iterator {
		return m_opt.begin();
	}
	auto end() -> const std::map<key_c, str_t>::iterator {
		return m_opt.end();
	}
};
