/** @file
 * @brief A TermList in a glass database.
 */
/* Copyright (C) 2007,2008,2009,2010,2011 Olly Betts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef XAPIAN_INCLUDED_GLASS_TERMLIST_H
#define XAPIAN_INCLUDED_GLASS_TERMLIST_H

#include <string>
#include "xapian/intrusive_ptr.h"
#include <xapian/positioniterator.h>
#include <xapian/types.h>

namespace Xapian {
	namespace Internal {
		class ExpandStats;
	}
}

#include "glass_database.h"
#include "termlist.h"
#include "glass_table.h"

/// A TermList in a glass database.
class GlassTermList : public TermList {
	void operator =(const GlassTermList &); /// Don't allow assignment.
	GlassTermList(const GlassTermList &); /// Don't allow copying.
	Xapian::Internal::intrusive_ptr<const GlassDatabase> db; /// The database we're reading data from.
	Xapian::docid did; /// The document id that this TermList is for.
	Xapian::termcount doclen; /// The length of document @a did.
	Xapian::termcount termlist_size; /// The number of entries in this termlist.
	std::string data; /// The tag value from the termlist table which holds the encoded termlist.
	/** Current position with the encoded tag value held in @a data.
	 *
	 *  If we've iterated to the end of the list, this gets set to NULL.
	 */
	const char * pos;
	const char * end; /// Pointer to the end of the encoded tag value.
	std::string current_term; /// The termname at the current position.
	Xapian::termcount current_wdf; /// The wdf for the term at the current position.
	/** The term frequency for the term at the current position.
	 *
	 *  This will have the value 0 if the term frequency has not yet been
	 *  looked up in the database (so it needs to be mutable).
	 */
	mutable Xapian::doccount current_termfreq;
public:
	/** Create a new GlassTermList object for document @a did_ in DB @a db_
	 *
	 *  @param throw_if_not_present  Specifies behaviour if document @a did_
	 *				     isn't present: if true then throw
	 *				     DocNotFoundError, otherwise the
	 *				     constructed GlassTermList object will
	 *				     signal at_end() before next() is called
	 *				     (normally at_end() isn't meaningful
	 *				     on a freshly constructed TermList).
	 */
	GlassTermList(Xapian::Internal::intrusive_ptr<const GlassDatabase> db_, Xapian::docid did_, bool throw_if_not_present = true);
	/** Return the length of this document.
	 *
	 *  This is a non-virtual method, used by GlassDatabase.
	 */
	Xapian::termcount get_doclength() const;
	/** Return the number of unique terms.
	 *
	 *  This is a non-virtual method, used by GlassDatabase.
	 */
	Xapian::termcount get_unique_terms() const;
	/** Return approximate size of this termlist.
	 *
	 *  For a GlassTermList, this value will always be exact.
	 */
	Xapian::termcount get_approx_size() const;
	/// Collate weighting information for the current term.
	void accumulate_stats(Xapian::Internal::ExpandStats & stats) const;
	/// Return the termname at the current position.
	std::string get_termname() const;
	/// Return the wdf for the term at the current position.
	Xapian::termcount get_wdf() const;
	/** Return the term frequency for the term at the current position.
	 *
	 *  In order to be able to support updating databases efficiently, we can't
	 *  store this value in the termlist table, so it has to be read from the
	 *  postlist table, which is relatively expensive (compared to reading the
	 *  wdf for example).
	 */
	Xapian::doccount get_termfreq() const;
	/** Advance the current position to the next term in the termlist.
	 *
	 *  The list starts before the first term in the list, so next(), skip_to()
	 *  or check() must be called before any methods which need the context of
	 *  the current position.
	 *
	 *  @return Always returns 0 for a GlassTermList.
	 */
	TermList * next();
	TermList * skip_to(const std::string & term);
	/// Return true if the current position is past the last term in this list.
	bool at_end() const;
	/// Return the length of the position list for the current position.
	Xapian::termcount positionlist_count() const;
	/// Return a PositionIterator for the current position.
	PositionList* positionlist_begin() const;
};

#endif // XAPIAN_INCLUDED_GLASS_TERMLIST_H
