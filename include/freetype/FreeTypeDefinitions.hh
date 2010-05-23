#ifndef __SMF_FREETYPEDEFINITIONS_HH
#define __SMF_FREETYPEDEFINITIONS_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 4, 2008
 *
 * File: FreeTypeDefinitions.hh
 *
 * Some variable, classes and enumerations used in working with the FreeType
 * rendering class.
 */

class FreeTypeRenderStyle {

public:

    virtual FreeTypeRenderStyle renderStyleDefault();
    virtual FreeTypeRenderStyle renderStyleMono();
    virtual FreeTypeRenderStyle renderStyleAliased();

private:

    /** Prevent any instance of FreeTypeRenderStyle from being created. @{ */
    FreeTypeRenderStyle();
    FreeTypeRenderStyle(const FreeTypeRenderStyle&);
    virtual ~FreeTypeRenderStyle();

    FreeTypeRenderStyle& operator=(const FreeTypeRenderStyle&);
    /** @} */

};	// class FreeTypeRenderStyle

#endif // __FREETYPEDEFINITIONS_HH
