// Copyright (c) 2008, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// Author: csilvers@google.com (Craig Silverstein)
//
// In addition to a TemplateDictionary, there is also a PerExpandData
// dictionary.  This dictionary holds information that applies to one
// call to Expand, such as whether to annotate the template expansion
// output.  A template dictionary is associated with a template (.tpl)
// file; a per-expand dictionary is associated to a particular call to
// Expand() in a .cc file.
//
// For (many) more details, see the doc/ directory.

#ifndef TEMPLATE_PER_EXPAND_DATA_H_
#define TEMPLATE_PER_EXPAND_DATA_H_

#include <stdlib.h>   // for NULL
#include <string.h>   // for strcmp
#include <sys/types.h>
#include <unordered_map>
#include <ctemplate/template_string.h>   // for StringHash

// NOTE: if you are statically linking the template library into your binary
// (rather than using the template .dll), set '/D CTEMPLATE_DLL_DECL='
// as a compiler flag in your project file to turn off the dllimports.
#ifndef CTEMPLATE_DLL_DECL
	#define CTEMPLATE_DLL_DECL  // @sobolev __declspec(dllimport)
#endif

namespace ctemplate {
class TemplateModifier;
class TemplateAnnotator;

class CTEMPLATE_DLL_DECL PerExpandData {
public:
	PerExpandData() : annotate_path_(NULL), annotator_(NULL), expand_modifier_(NULL), map_(NULL) 
	{
	}
	~PerExpandData();

	// Indicate that annotations should be inserted during template expansion.
	// template_path_start - the start of a template path.  When
	// printing the filename for template-includes, anything before and
	// including template_path_start is elided.  This can make the
	// output less dependent on filesystem location for template files.
	void SetAnnotateOutput(const char* template_path_start) {
		annotate_path_ = template_path_start;
	}

	// Whether to annotate the expanded output.
	bool annotate() const {
		return annotate_path_ != NULL;
	}

	// The annotate-path; undefined if annotate() != true
	const char* annotate_path() const {
		return annotate_path_;
	}

	// This sets the TemplateAnnotator to be used when annotating is on.
	// This allows you to override the default text-based annotator
	// that will be used if you do not call this.  The passed annotator
	// will be aliased by this object and returned by annotator().
	// Passing NULL has the special behavior of causing annotator() to
	// revert to returning its built-in instance.
	void SetAnnotator(TemplateAnnotator* annotator) {
		annotator_ = annotator;
	}

	// This returns the TemplateAnnotator to be used when annotating is on.
	// The value returned will be either an instance previously provided
	// to SetAnnotator() or the callable built-in text-based annotator.
	TemplateAnnotator* annotator() const;

	// This is a TemplateModifier to be applied to all templates
	// expanded via this call to Expand().  That is, this modifier is
	// applies to the template (.tpl) file we expand, as well as
	// sub-templates that are expanded due to {{>INCLUDE}} directives.
	// Caller is responsible for ensuring that modifier exists for the
	// lifetime of this object.
	void SetTemplateExpansionModifier(const TemplateModifier* modifier) {
		expand_modifier_ = modifier;
	}

	const TemplateModifier* template_expansion_modifier() const {
		return expand_modifier_;
	}

	// Store data in this structure, to be used by template modifiers
	// (see template_modifiers.h).  Call with value set to NULL to clear
	// any value previously set.  Caller is responsible for ensuring key
	// and value point to valid data for the lifetime of this object.
	void InsertForModifiers(const char* key, const void * value);

	// Retrieve data specific to this Expand call. Returns NULL if key
	// is not found.  This should only be used by template modifiers.
	const void * LookupForModifiers(const char* key) const;

	// Same as Lookup, but casts the result to a c string.
	const char* LookupForModifiersAsString(const char* key) const {
		return static_cast<const char*>(LookupForModifiers(key));
	}

private:
	struct DataEq {
		bool operator()(const char* s1, const char* s2) const;
	};

	typedef std::unordered_map<const char*, const void *, StringHash, DataEq> DataMap;

	const char* annotate_path_;
	TemplateAnnotator* annotator_;
	const TemplateModifier* expand_modifier_;
	DataMap* map_;

	PerExpandData(const PerExpandData&); // disallow evil copy constructor
	void operator=(const PerExpandData&); // disallow evil operator=
};
}

#endif  // TEMPLATE_PER_EXPAND_DATA_H_
