/**********************************************************************
** This program is part of 'MOOSE', the
** Messaging Object Oriented Simulation Environment.
**           Copyright (C) 2003-2013 Upinder S. Bhalla. and NCBS
** It is made available under the terms of the
** GNU Lesser General Public License version 2.1
** See the file COPYING.LIB for the full notice.
**********************************************************************/
#ifndef _FIELD_ELEMENT_H
#define _FIELD_ELEMENT_H

/**
 * Specialization of Element class, used to look up array fields within
 * objects when those fields each need to have independent Element 
 * capabilies such as messaging and subfield lookup.
 * Made automatically by Elements which have such fields.
 */
class FieldElement: public Element
{
	public:
		FieldElement( Id id, const Cinfo* c, const string& name,
			char* ( *lookupField )( char*, unsigned int ),
			void( *setNumField )( unsigned int num ),
			unsigned int ( *getNumField )() const
		);

		~FieldElement();

		/// Virtual: Returns number of field entries for specified data
		unsigned int numField( unsigned int rawIndex ) const;

		/**
		 * Virtual: True if this is a FieldElement having an array of
		 * fields on each data entry. So true in this case.
		 */
		bool hasFields() const {
			return true;
		}

		/////////////////////////////////////////////////////////////////
		// data access stuff
		/////////////////////////////////////////////////////////////////

		/**
		 * virtual.
		 * Looks up specified field field entry. First it finds the
		 * appropriate data entry from the rawIndex. Then it looks up
		 * the field using the lookupField. 
		 * Returns the data entry specified by the rawIndex, fieldIndex. 
		 *
		 * Note that the index is NOT a
		 * DataId: it is instead the raw index of the data on the current
		 * node. Index is also NOT the character offset, but the index
		 * to the data array in whatever type the data may be.
		 *
		 * The DataId has to be filtered through the nodeMap to
		 * find a) if the entry is here, and b) what its raw index is.
		 *
		 * Returns 0 if either index is out of range.
		 */
		char* data( unsigned int rawIndex, 
						unsigned int fieldIndex = 0 ) const;

		/**
		 * virtual
		 * Changes the number of entries in the data. Not permitted for
		 * FieldElements since they are just fields on the data.
		 */
		void resize( unsigned int newNumData );

		/**
		 * virtual.
		 * Changes the number of fields on the specified data entry.
		 */
		void resizeField( 
				unsigned int rawIndex, unsigned int newNumField );


	private:
		Id parent_;
		char* ( *lookupField )( char*, unsigned int );
		void( *setNumField )( unsigned int num );
		unsigned int ( *getNumField )() const;
};

#endif // _FIELD_ELEMENT_H