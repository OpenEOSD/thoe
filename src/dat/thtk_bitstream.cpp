#include "../sys/oserror.hpp"
#include "../sattypes.hpp"

#include "thtk/thtk.h"

#ifdef _WIN32
# define THTK_OPEN_FILE thtk_io_open_file_w
# define THDAT_DETECT thdat_detect_w
 typedef const wchar_t* thtk_path_t;
#else
# define THTK_OPEN_FILE thtk_io_open_file
# define THDAT_DETECT thdat_detect
 typedef const char* thtk_path_t;
#endif


class TH_Entry {

	thtk_io_t    *m_io;
	thtk_error_t *m_err;
	thdat_t      *m_dat;
	int32_t       m_idx, m_size, m_zsize;
	cstr_t        m_name;

	TH_Entry(thdat_t *dat, int32_t idx, cstr_t name) {
		m_dat = dat, m_name = name;
		m_idx = idx, m_size = m_zsize = (idx == -1 ? -1 : 0);
		m_err = NULL, m_io = NULL;
	}
public:
	~TH_Entry() {
		if (m_io) thtk_io_close(m_io);
	}
	auto index() -> const int32_t { return m_idx; }
	auto  size() -> const int32_t {
		if (!m_size && !m_err && -1 == (
		     m_size = thdat_entry_get_size(m_dat, m_idx, &m_err)
		)) Sys_Err(thtk_error_message(m_err));
		return m_size;
	}
	auto compressed_size() -> const int32_t {
		if (!m_zsize && !m_err && -1 == (
		     m_zsize = thdat_entry_get_zsize(m_dat, m_idx, &m_err)
		)) Sys_Err(thtk_error_message(m_err));
		return m_zsize;
	}
	auto name() -> const char* {
		if (!m_name && !m_err && !(
		     m_name = thdat_entry_get_name(m_dat, m_idx, &m_err)
		)) Sys_Err(thtk_error_message(m_err));
		return m_name;
	}
	auto extract() -> U8ClampVec {
		int32_t sz = size();
		uint8_t *buf = new uint8_t[sz];
		if (m_idx != -1 && !m_err &&
		   (m_io = thtk_io_open_memory(buf, sz, &m_err)) &&
		  -1 == (thdat_entry_read_data(m_dat, m_idx, m_io, &m_err))
		) Sys_Err(thtk_error_message(m_err));
		return U8ClampVec(buf, buf + sz);
	}
	friend class TH_Dat;
	friend class TH_Bitstream;
};

class TH_Bitstream {

	thtk_io_t    *m_io;
	const bool    m_ad;
	thtk_error_t *m_err;

	TH_Bitstream(thtk_io_t *io) : m_io(io), m_ad(false) {}
public:
	TH_Bitstream(thtk_path_t path) : m_ad(true)
	{
		if (!(m_io = THTK_OPEN_FILE(path, "rb", &m_err))) {
			Sys_Err(thtk_error_message(m_err));
			Sys_ExitFail();
		}
	}
	TH_Bitstream(void *buf, size_t size) : m_ad(true)
	{
		if (!(m_io = thtk_io_open_memory(buf, size, &m_err))) {
			Sys_Err(thtk_error_message(m_err));
			Sys_ExitFail();
		}
	}
	~TH_Bitstream() {
		if (m_io && m_ad) thtk_io_close(m_io);
	}

	ssize_t read(void *buf, size_t count) {
		ssize_t n = thtk_io_read(m_io, buf, count, &m_err);
		if (n == -1) {
			Sys_Err(thtk_error_message(m_err));
			Sys_ExitFail();
		}
		return n;
	}
	ssize_t read(TH_Entry& entry) {
		ssize_t sz = thdat_entry_read_data(entry.m_dat, entry.m_idx, m_io, &m_err);
		if(sz == -1) {
			Sys_Err(thtk_error_message(m_err));
			Sys_ExitFail();
		}
		return sz;
	}
	off_t seek(off_t offset, int whence) {
		off_t n = thtk_io_seek(m_io, offset, whence, &m_err);
		if (n == -1) {
			Sys_Err(thtk_error_message(m_err));
			Sys_ExitFail();
		}
		return n;
	}
	friend class TH_Dat;
};

class TH_Dat {

	thtk_io_t    *m_io;
	thtk_error_t *m_err;
	thdat_t      *m_dat;
	uint32_t      m_ver, m_ecount;

public:
	TH_Dat(thtk_path_t dat_file) : m_err(NULL), m_ver(0), m_ecount(0)
	{
		uint32_t out[4]; // 32 0 0 0

		if ( !(m_io = THTK_OPEN_FILE(dat_file, "rb", &m_err)) ||
			THDAT_DETECT(dat_file, m_io, out, &m_ver, &m_err) ||
			 !(m_dat = thdat_open(m_ver, m_io, &m_err))
		) {
			Sys_Err(thtk_error_message(m_err));
			Sys_ExitFail();
		}
	}
	TH_Dat(thtk_path_t dat_file, uint32_t version) : m_err(NULL), m_ver(version), m_ecount(0)
	{
		if( !(m_io  = THTK_OPEN_FILE(dat_file, "rb", &m_err)) ||
			!(m_dat = thdat_open(version, m_io, &m_err))
		) {
			Sys_Err(thtk_error_message(m_err));
			Sys_ExitFail();
		}
	}
	~TH_Dat() {
		if (m_dat) thdat_free(m_dat);
		if (m_io) thtk_io_close(m_io);
	}

	auto   bitstream() -> const TH_Bitstream { return TH_Bitstream(m_io); }
	auto     version() -> const uint32_t     { return m_ver; }
	auto entry_count() -> const uint32_t     {
		if (!m_ecount) {
			ssize_t s = thdat_entry_count(m_dat, &m_err);
			m_ecount  = s;
		}
		return m_ecount;
	}
	auto entry(int32_t index) -> const TH_Entry { return TH_Entry(m_dat, index, NULL); }
	auto entry( cstr_t name ) -> const TH_Entry {
		ssize_t idx = thdat_entry_by_name(m_dat, name, &m_err);
		if (idx == -1)
			Sys_Err(thtk_error_message(m_err));
		return TH_Entry(m_dat, idx, name);
	}
};
