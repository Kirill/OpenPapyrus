/** @file
 * @brief GlassVersion class
 */
/* Copyright (C) 2006,2007,2008,2009,2010,2013,2014,2015,2016,2018 Olly Betts
 * Copyright (C) 2011 Dan Colish
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef XAPIAN_INCLUDED_GLASS_VERSION_H
#define XAPIAN_INCLUDED_GLASS_VERSION_H

#include "glass_changes.h"
#include "glass_defs.h"
#include "internaltypes.h"
#include "min_non_zero.h"

namespace Glass {
class RootInfo {
	glass_block_t root;
	uint level;
	glass_tablesize_t num_entries;
	bool root_is_fake;
	bool sequential;
	uint blocksize;
	/// Should be >= 4 or 0 for no compression.
	uint4 compress_min;
	std::string fl_serialised;
public:
	void init(uint blocksize_, uint4 compress_min_);
	void serialise(std::string &s) const;
	bool unserialise(const char ** p, const char * end);
	glass_block_t get_root() const { return root; }
	int get_level() const { return int(level); }
	glass_tablesize_t get_num_entries() const { return num_entries; }
	bool get_root_is_fake() const { return root_is_fake; }
	bool get_sequential() const { return sequential; }
	uint get_blocksize() const 
	{
		AssertRel(blocksize, >=, GLASS_MIN_BLOCKSIZE);
		AssertRel(blocksize, <=, GLASS_MAX_BLOCKSIZE);
		return blocksize;
	}
	uint4 get_compress_min() const { return compress_min; }
	const std::string & get_free_list() const { return fl_serialised; }
	void set_level(int level_) { level = uint(level_); }
	void set_num_entries(glass_tablesize_t n) { num_entries = n; }
	void set_root_is_fake(bool f) { root_is_fake = f; }
	void set_sequential(bool f) { sequential = f; }
	void set_root(glass_block_t root_) { root = root_; }
	void set_blocksize(uint b) 
	{
		AssertRel(b, >=, GLASS_MIN_BLOCKSIZE);
		AssertRel(b, <=, GLASS_MAX_BLOCKSIZE);
		blocksize = b;
	}
	void set_free_list(const std::string & s) 
	{
		fl_serialised = s;
	}
};
}

using Glass::RootInfo;

/** The GlassVersion class manages the revision files.
 *
 *  The "iamglass" file (currently) contains a "magic" string identifying
 *  that this is a glass database, a database format version number, the UUID
 *  of the database, the revision of the database, and the root block info for
 *  each table.
 */
class GlassVersion {
	glass_revision_number_t rev;
	RootInfo root[Glass::MAX_];
	RootInfo old_root[Glass::MAX_];
	Uuid uuid; /// The UUID of this database.
	/** File descriptor.
	 *
	 *  When committing, this hold the file descriptor of the new changes file
	 *  between the call to the write() and sync() methods.
	 *
	 *  For a single-file database (when db_dir.empty()), this holds the fd of
	 *  that file for use in read().
	 */
	int fd;
	/** Offset into the file at which the version data starts.
	 *
	 *  Will be 0, except for an embedded multi-file database.
	 */
	off_t offset;
	std::string db_dir; /// The database directory.
	GlassChanges * changes;
	Xapian::doccount doccount; /// The number of documents in the database.
	Xapian::totallength total_doclen; /// The total of the lengths of all documents in the database.
	Xapian::docid last_docid; /// Greatest document id ever used in this database.
	Xapian::termcount doclen_lbound; /// A lower bound on the smallest document length in this database.
	Xapian::termcount doclen_ubound; /// An upper bound on the greatest document length in this database.
	Xapian::termcount wdf_ubound; /// An upper bound on the greatest wdf in this database.
	Xapian::termcount spelling_wordfreq_ubound; /// An upper bound on the spelling wordfreq in this database.
	mutable glass_revision_number_t oldest_changeset; /// Oldest changeset removed when max_changesets is set
	std::string serialised_stats; /// The serialised database stats.
	void serialise_stats(); // Serialise the database stats.
	void unserialise_stats(); // Unserialise the database stats.
public:
	explicit GlassVersion(const std::string & db_dir_) : rev(0), fd(-1), offset(0), db_dir(db_dir_), changes(NULL),
		doccount(0), total_doclen(0), last_docid(0), doclen_lbound(0), doclen_ubound(0), wdf_ubound(0), spelling_wordfreq_ubound(0), oldest_changeset(0) 
	{
	}
	explicit GlassVersion(int fd_);
	~GlassVersion();
	/** Create the version file. */
	void create(uint blocksize);
	void set_changes(GlassChanges * changes_) 
	{
		changes = changes_;
	}
	/** Read the version file and check it's a version we understand.
	 *
	 *  On failure, an exception is thrown.
	 */
	void read();
	void cancel();
	const std::string write(glass_revision_number_t new_rev, int flags);
	bool sync(const std::string & tmpfile, glass_revision_number_t new_rev, int flags);
	glass_revision_number_t get_revision() const { return rev; }
	const RootInfo & get_root(Glass::table_type tbl) const { return root[tbl]; }
	RootInfo * root_to_set(Glass::table_type tbl) { return &root[tbl]; }
	/// Return pointer to 16 byte UUID.
	const char * get_uuid() const { return uuid.data(); }
	/// Return UUID in the standard 36 character string format.
	std::string get_uuid_string() const { return uuid.to_string(); }
	Xapian::doccount get_doccount() const { return doccount; }
	Xapian::totallength get_total_doclen() const { return total_doclen; }
	Xapian::docid get_last_docid() const { return last_docid; }
	Xapian::termcount get_doclength_lower_bound() const { return doclen_lbound; }
	Xapian::termcount get_doclength_upper_bound() const { return doclen_ubound; }
	Xapian::termcount get_wdf_upper_bound() const { return wdf_ubound; }
	Xapian::termcount get_spelling_wordfreq_upper_bound() const { return spelling_wordfreq_ubound; }
	glass_revision_number_t get_oldest_changeset() const { return oldest_changeset; }
	Xapian::termcount get_unique_terms_lower_bound() const 
	{
		if(total_doclen == 0) return 0;
		Assert(doclen_lbound != 0);
		Assert(wdf_ubound != 0);
		return (doclen_lbound - 1) / wdf_ubound + 1;
	}
	void set_last_docid(Xapian::docid did) { last_docid = did; }
	void set_oldest_changeset(glass_revision_number_t changeset) const { oldest_changeset = changeset; }
	void set_spelling_wordfreq_upper_bound(Xapian::termcount ub) { spelling_wordfreq_ubound = ub; }
	void add_document(Xapian::termcount doclen) 
	{
		++doccount;
		doclen_lbound = min_non_zero(doclen_lbound, doclen);
		doclen_ubound = std::max(doclen_ubound, doclen);
		total_doclen += doclen;
	}
	void delete_document(Xapian::termcount doclen) 
	{
		--doccount;
		total_doclen -= doclen;
		// If the database no longer contains any postings, we can reset
		// doclen_lbound, doclen_ubound and wdf_ubound.
		if(total_doclen == 0) {
			doclen_lbound = 0;
			doclen_ubound = 0;
			wdf_ubound = 0;
		}
	}
	void check_wdf(Xapian::termcount wdf) 
	{
		if(wdf > wdf_ubound) 
			wdf_ubound = wdf;
	}
	Xapian::docid get_next_docid() 
	{
		return ++last_docid;
	}
	/** Merge the database stats.
	 *
	 *  Used by compaction.
	 */
	void merge_stats(const GlassVersion & o);
	bool single_file() const { return db_dir.empty(); }
	off_t get_offset() const { return offset; }
};

#endif // XAPIAN_INCLUDED_GLASS_VERSION_H
