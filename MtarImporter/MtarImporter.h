#pragma once

#include <maya/MPxFileTranslator.h>

#include <iosfwd>

// The animImport command object
//
class animImport : public MPxFileTranslator 
{
public:
	animImport();
	~animImport() override;

	MStatus		 doIt(const MArgList& argList);
	static void* creator();

	MStatus		reader(const MFileObject& file, const MString& optionsString, FileAccessMode mode) override;

	bool		haveReadMethod() const override;
	bool		haveWriteMethod() const override;
	bool		canBeOpened() const override;
	MString 	defaultExtension() const override;
	MFileKind	identifyFile(const MFileObject& fileName, const char* buffer, short size) const override;
private:
	MStatus		importAnim(std::ifstream&, const MString&);
};