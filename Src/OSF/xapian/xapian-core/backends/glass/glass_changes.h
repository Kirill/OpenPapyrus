/** @file
 * @brief Glass changesets
 */
/* Copyright 2014 Olly Betts
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 */
#ifndef XAPIAN_INCLUDED_GLASS_CHANGES_H
#define XAPIAN_INCLUDED_GLASS_CHANGES_H

#include "glass_defs.h"
#include <string>

class GlassChanges {
	int changes_fd; /// File descriptor to write changeset to (or -1 for none).
	std::string changes_stem;
	/** The maximum number of changesets to keep.
	 *
	 *  If a slave is more than this number of changesets behind, it will need
	 *  to be sent a full copy of the database (which can be more efficient if
	 *  a lot has changed).
	 */
	glass_revision_number_t max_changesets;
	/** The oldest changeset which might exist on disk.
	 *
	 *  Used to optimise removal of old changesets by giving us a point to
	 *  start looking for ones to delete.
	 */
	glass_revision_number_t oldest_changeset;
public:
	explicit GlassChanges(const std::string & db_dir) : changes_fd(-1), changes_stem(db_dir + "/changes"), oldest_changeset(0) 
	{
	}
	~GlassChanges();
	GlassChanges * start(glass_revision_number_t old_rev, glass_revision_number_t rev, int flags);
	void write_block(const char * p, size_t len);
	void write_block(const std::string & s) 
	{
		write_block(s.data(), s.size());
	}
	void set_oldest_changeset(glass_revision_number_t rev) 
	{
		oldest_changeset = rev;
	}
	glass_revision_number_t get_oldest_changeset() const { return oldest_changeset; }
	void commit(glass_revision_number_t new_rev, int flags);
	static void check(const std::string & changes_file);
};

#endif // XAPIAN_INCLUDED_GLASS_CHANGES_H
