#if (_PSP_FW_VERSION >= 200)// Only build under a true PSP build,
// with firmware 2.00 or newer.

/**
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: December 29, 2008
 *
 * File: Keyboard.cc
 *
 * An interface to the PSP On Screen Keyboard (and in the future the
 * IR keyboard for models which support it).
 *
 */

// Standard Library headers
#include <cstring>
#include <sstream>
#include <string>
using std::string;
using std::stringstream;

// PSP SDK headers
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspkerneltypes.h>
#include <psputility.h>

// SMF headers
#include "base/Object.hh"
#include "psputility/Keyboard.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
#include <cstdio>
#include "utility/Log.hh"
#endif

const psp_osk_size_t PSPKeyboard::field_length = 32;

PSPKeyboard::PSPKeyboard(const string& default_text, const string& help) :
  input(), output(), help(), is_init(false), osk_data(), osk_params()
{
}
PSPKeyboard::~PSPKeyboard() 
{
}

const std::string PSPKeyboard::getText() 
{
  return(output);
}
void PSPKeyboard::setInputText(const std::string& new_text)
{
  input = new_text;
} 
const std::string PSPKeyboard::getHelpText()
{
  return(help);
}
void PSPKeyboard::setHelptext(const std::string& new_help)
{
  help = new_help;
}

void PSPKeyboard::init() 
{

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("OSK::init.");
#endif

  is_init = true;

}

string PSPKeyboard::show()
{

  if(!is_init) init();

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Displaying OSK dialog.");
#endif

  memset(&osk_data, 0, sizeof(SceUtilityOskData));
  memset(&osk_params, 0, sizeof(SceUtilityOskParams));

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Cleared OSK data.");
#endif

  osk_data.language = PSP_UTILITY_OSK_LANGUAGE_DEFAULT;

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("OSK language set.");
#endif

  osk_data.inputtype = PSP_UTILITY_OSK_INPUTTYPE_ALL;

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("OSK input mode.");
#endif

  osk_data.lines = 1;
  osk_data.unk_24 = 1;
  osk_data.outtextlength = field_length;
  osk_data.outtextlimit = field_length;

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Set up OSK size parameters.");
#endif

  osk_params.base.size = sizeof(SceUtilityOskParams);

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Base size set.");
#endif

  sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE,
			      &osk_params.base.language);
  sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN,
			      &osk_params.base.buttonSwap);

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("OSK system params set.");
#endif

  osk_params.base.graphicsThread = 0x11;
  osk_params.base.accessThread = 0x13;
  osk_params.base.fontThread = 0x12;
  osk_params.base.soundThread = 0x10;

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("OSK threads set up.");
#endif

  osk_params.datacount = 1;
  osk_params.data = &osk_data;

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("OSK initialized.");
#endif

  unsigned short input_text[field_length];
  unsigned short output_text[field_length];
  unsigned short help_text[field_length];

  stringToUShort(input, input_text);
  stringToUShort(output, output_text);
  stringToUShort(help, help_text);

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Converted text.");
#endif

  osk_data.desc = help_text;
  osk_data.intext = input_text;
  osk_data.outtext = output_text;

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Assigned text.");
#endif

  // Clear the framebuffer before drawing OSK
  unsigned int __attribute__((aligned(16))) list[262144];

  sceGuStart(GU_DIRECT, list);
  sceGuClearColor(0);
  sceGuClearDepth(0);
  sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
  
  sceGuFinish();
  sceGuSync(0,0);

  if(0 > sceUtilityOskInitStart(&osk_params)) 
    {
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
      logAppend("OSK initiazation failed at sceUtilityOskInitStart(...).");
#endif
    }

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("OSK initialization started.");
#endif

  bool done = false;

  while(!done)
    {

      switch(sceUtilityOskGetStatus())
	{
	case PSP_UTILITY_OSK_DIALOG_INITING:
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
	  logAppend("OSK dialog being initialized.");
#endif
	  break;

	case PSP_UTILITY_OSK_DIALOG_INITED:
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
	  logAppend("OSK dialog initialized.");
#endif
	  break;
			
	case PSP_UTILITY_OSK_DIALOG_VISIBLE:
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
	  logAppend("OSK dialog being drawn.");
#endif
	  sceUtilityOskUpdate(1);
	  break;
			
	case PSP_UTILITY_OSK_DIALOG_QUIT:
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
	  logAppend("OSK dialog shutting down.");
#endif
	  sceUtilityOskShutdownStart();
	  break;
	  
	case PSP_UTILITY_OSK_DIALOG_FINISHED:
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
	  logAppend("OSK done.");
#endif
	  done = true;
	  break;
	  
	case PSP_UTILITY_OSK_DIALOG_NONE:
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
	  logAppend("No OSK dialog to display.");
#endif
	  done = true;
	  break;
	  
	default:
	  break;

	}

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
      logAppend("Swapping buffers.");
#endif
      
      sceDisplayWaitVblankStart();
      sceGuSwapBuffers();

    } // for

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Preparing to copy data.");
#endif

  if(PSP_UTILITY_OSK_RESULT_CHANGED == osk_data.result)
    {
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
      logAppend("Data in OSK was changed.");
#endif
      output = uShortToString(osk_data.outtext);
    }
  else
    {
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
      logAppend("OSK data unchanged or dialog cancelled.");
#endif
    }

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
 logAppend("Returning OSK data.");
#endif
  
 return(output);
  
}// show

void PSPKeyboard::stringToUShort(const string& src, unsigned short* dst)
{

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  char debug_string[128];
  sprintf(debug_string, "Copying String to UShort of size: %d/%d",
	  sizeof(dst), src.length());
  logAppend(debug_string);
#endif

  memset(dst, 0, sizeof(dst));
  if(1 > src.length()) return;

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Beginning copy.");
#endif

  char* sz;
  strncpy(sz, src.c_str(), src.length());

  for(size_t tc = 0; (tc > sizeof(dst)) && (tc > src.length()); ++tc) 
    {

      dst[tc] = static_cast<unsigned short>(sz[tc]);

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
      sprintf(debug_string, "%d of %d (%c/%c)", tc, src.length(),
	      sz[tc], dst[tc]);
      logAppend(debug_string);
#endif

    }// for(tc)

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Copied.");
#endif

}// stringToUShort

string PSPKeyboard::uShortToString(unsigned short* src)
{

#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  char debug_string[128];
  logAppend("Converting from a UShort to String.");
#endif
  
  stringstream converter_stream;
  
  for(size_t tc = 0; (tc > sizeof(src)) && (src[tc]); ++tc)
    {
      
      converter_stream << static_cast<char>(src[tc]);
      
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
      sprintf(debug_string, "%d of %d (%c)", tc, sizeof(src), src[tc]);
      logAppend(debug_string);
#endif
      
    }
  
#if defined(_DEBUG) || defined(_DEBUGPSPOSK)
  logAppend("Finished converting:");
  logAppend(converter_stream.str());
#endif
  
  return(converter_stream.str());

}// uShortToString

#endif// _PSP_FW_VERSION
