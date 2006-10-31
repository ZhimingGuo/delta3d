/*
* Delta3D Open Source Game and Simulation Engine
* Copyright (C) 2006, Alion Science and Technology, BMH Operation
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 2.1 of the License, or (at your option)
* any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
* details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* @author Curtiss Murphy
*/

#include "HUDComponent.h"

#include <dtCore/globals.h>
#include <dtUtil/exception.h>

#include <dtGame/basemessages.h>
#include <dtGame/messagetype.h>
#include <dtGame/exceptionenum.h>

#include <dtDAL/project.h>

#if defined (WIN32) || defined (_WIN32) || defined (__WIN32__)
#define snprintf _snprintf
#endif

//////////////////////////////////////////////////////////////////////////
HUDComponent::HUDComponent(dtCore::DeltaWin *win, const std::string &name) :
   dtGame::GMComponent(name),
   mUnHandledMessages(0)
{
   SetupGUI(win);
}

//////////////////////////////////////////////////////////////////////////
HUDComponent::~HUDComponent()
{
   mGUI->ShutdownGUI();
}


//////////////////////////////////////////////////////////////////////////

// TUTORIAL - IMPLEMENT ProcessMessage().  TRAP MESSAGES.


//////////////////////////////////////////////////////////////////////////
void HUDComponent::UpdateSimTime(double newTime)
{
   char clin[HUDCONTROLMAXTEXTSIZE]; // general buffer to print

   // Sim Time
   snprintf(clin, HUDCONTROLMAXTEXTSIZE, "SimTime: %.2f", newTime);
   UpdateStaticText(mSimTimeText, clin);
}

//////////////////////////////////////////////////////////////////////////
void HUDComponent::UpdateNumMessages(int messageCount)
{
   char clin[HUDCONTROLMAXTEXTSIZE]; // general buffer to print

   // Num Messages
   snprintf(clin, HUDCONTROLMAXTEXTSIZE, "Num Msgs: %lu", mUnHandledMessages);
   UpdateStaticText(mNumMessagesText, clin);
}

//////////////////////////////////////////////////////////////////////////
void HUDComponent::UpdateLastMessageName(const std::string &messageName)
{
   char clin[HUDCONTROLMAXTEXTSIZE]; // general buffer to print

   // Last Message
   snprintf(clin, HUDCONTROLMAXTEXTSIZE, "Last Msg: %s", messageName.c_str());
   UpdateStaticText(mLastMessageText, clin);
}

//////////////////////////////////////////////////////////////////////////
void HUDComponent::OnAddedToGM()
{
   GetGameManager()->GetScene().AddDrawable(GetGUIDrawable().get());
}

//////////////////////////////////////////////////////////////////////////
void HUDComponent::SetupGUI(dtCore::DeltaWin *win)
{
   try
   {
      // Initialize CEGUI
      mGUI = new dtGUI::CEUIDrawable(win);

      // BEGIN - MAKE THIS PART OF BaseHUDComponent ???
      // probably have params for the scheme, the default font, and main win name

      // get our scheme path
      std::string scheme = "gui/schemes/WindowsLookSkin.scheme";
      std::string path = dtCore::FindFileInPathList(scheme);
      if(path.empty())
      {
         EXCEPT(dtGame::ExceptionEnum::GENERAL_GAMEMANAGER_EXCEPTION,
            "Failed to find the scheme file.");
      }

      std::string dir = path.substr(0, path.length() - (scheme.length() - 3));
      dtUtil::FileUtils::GetInstance().PushDirectory(dir);
      CEGUI::SchemeManager::getSingleton().loadScheme(path);
      dtUtil::FileUtils::GetInstance().PopDirectory();

      CEGUI::WindowManager *wm = CEGUI::WindowManager::getSingletonPtr();
      CEGUI::System::getSingleton().setDefaultFont("Tahoma-12");
      mMainWindow = wm->createWindow("DefaultGUISheet", "root");
      CEGUI::System::getSingleton().setGUISheet(mMainWindow);

      // END - MAKE THIS PART OF BaseHUDComponent ???


      // main HUD window for drawing, covers full window size
      mOverlay = static_cast<CEGUI::StaticImage*>(
         wm->createWindow("WindowsLook/StaticImage", "Main Overlay"));
      mMainWindow->addChildWindow(mOverlay);
      mOverlay->setPosition(CEGUI::Point(0.0f, 0.0f));
      mOverlay->setSize(CEGUI::Size(1.0f, 1.0f));
      mOverlay->setFrameEnabled(false);
      mOverlay->setBackgroundEnabled(false);

      // Sim Time
      mSimTimeText = CreateText("Sim Time", mOverlay, "",
         5.0f, 20.0f, 200.0f, 25.0f);
      mSimTimeText->setTextColours(CEGUI::colour(1.0, 1.0, 1.0));

      // Num Messages
      mNumMessagesText = CreateText("Num Messages", mOverlay, "",
         5.0f, 45.0f, 200.0f, 25.0f);
      mNumMessagesText->setTextColours(CEGUI::colour(1.0, 1.0, 1.0));

      // Last Message
      mLastMessageText = CreateText("Last Message", mOverlay, "Last Msg:",
         5.0f, 70.0f, 300.0f, 25.0f);
      mLastMessageText->setTextColours(CEGUI::colour(1.0, 1.0, 1.0));

      // Note - don't forget to add the cegui drawable to the scene after this method, or you get nothing.
   }
   catch(CEGUI::Exception &e)
   {
      std::ostringstream oss;
      oss << "CEGUI while setting up GUI: " << e.getMessage().c_str();
      EXCEPT(dtGame::ExceptionEnum::GENERAL_GAMEMANAGER_EXCEPTION, oss.str());
   }

}

//////////////////////////////////////////////////////////////////////////
void HUDComponent::UpdateStaticText(CEGUI::StaticText *textControl, char *newText,
                                  float red, float blue, float green, float x, float y)
{
   if (textControl != NULL)
   {
      // text and color
      if (newText != NULL && textControl->getText() != std::string(newText))
      {
         textControl->setText(newText);
         if (red >= 0.00 && blue >= 0.0 && green >= 0.0)
            textControl->setTextColours(CEGUI::colour(red, blue, green));
      }
      // position
      if (x > 0.0 && y > 0.0)
      {
         CEGUI::Point position = textControl->getPosition();
         CEGUI::Point newPos(x, y);
         if (position != newPos)
            textControl->setPosition(newPos);
      }
   }
}

//////////////////////////////////////////////////////////////////////////
CEGUI::StaticText * HUDComponent::CreateText(const std::string &name, CEGUI::StaticImage *parent, const std::string &text,
                                 float x, float y, float width, float height)
{
   CEGUI::WindowManager *wm = CEGUI::WindowManager::getSingletonPtr();

   // create base window and set our default attribs
   CEGUI::StaticText* result = static_cast<CEGUI::StaticText*>(wm->createWindow("WindowsLook/StaticText", name));
   parent->addChildWindow(result);
   result->setMetricsMode(CEGUI::Absolute);
   result->setText(text);
   result->setPosition(CEGUI::Point(x, y));
   result->setSize(CEGUI::Size(width, height));
   result->setFrameEnabled(false);
   result->setBackgroundEnabled(false);
   result->setHorizontalAlignment(CEGUI::HA_LEFT);
   result->setVerticalAlignment(CEGUI::VA_TOP);
   result->show();

   return result;
}

