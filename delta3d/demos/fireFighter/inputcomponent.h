/* -*-c++-*-
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
 * William E. Johnson II
 */
#ifndef DELTA_FIRE_FIGHTER_INPUT_COMPONENT
#define DELTA_FIRE_FIGHTER_INPUT_COMPONENT

#include <dtGame/baseinputcomponent.h>
#include <fireFighter/export.h>
#include <fireFighter/exception.h>

// Forward declarations
class GameState;
class GameItemActor;
class PlayerActor;

namespace dtCore
{
#ifdef _DEBUG
   class FlyMotionModel;
#else 
   class CollisionMotionModel;
#endif
}

namespace dtAudio
{
   class Sound;
}

class FIRE_FIGHTER_EXPORT InputComponent : public dtGame::BaseInputComponent
{
   public:

      static const std::string &NAME;

      /// Constructor
      InputComponent(const std::string &name = NAME);

      /**
       * Handles key presses
       */
      virtual bool HandleKeyPressed(const dtCore::Keyboard* keyboard, 
                                    Producer::KeyboardKey key,
                                    Producer::KeyCharacter character);

      /**
       * Handles key releases
       */
      virtual bool HandleKeyReleased(const dtCore::Keyboard* keyboard, 
                                     Producer::KeyboardKey key,
                                     Producer::KeyCharacter character);

      /**
       * Handles button presses
       */
      virtual bool HandleButtonPressed(const dtCore::Mouse* mouse, 
                                       dtCore::Mouse::MouseButton button);

      /**
       * Handles button releases
       */
      virtual bool HandleButtonReleased(const dtCore::Mouse* mouse, 
         dtCore::Mouse::MouseButton button);

      /**
       * Handles incoming messages
       */
      virtual void ProcessMessage(const dtGame::Message &message);

      /**
       * Returns the current state the game is in
       * @return mCurrentState
       */
      GameState& GetCurrentGameState() { return *mCurrentState; }


      /**
       * const overload
       * Returns the current state the game is in
       * @return mCurrentState
       */
      const GameState& GetCurrentGameState() const { return *mCurrentState; }

      /**
       * Called when this component is added to the game manager
       */
      virtual void OnAddedToGM();

      /**
       * Accessor to the player actor
       * @return mPlayer
       */
      const PlayerActor& GetPlayerActor() const { return *mPlayer; }

      /**
       * Accessor to the player actor
       * @return mPlayer
       */
      PlayerActor& GetPlayerActor() { return *mPlayer; }

   protected:

      /// Destructor
      virtual ~InputComponent();

   private:

      /**
       * Helper method to quickly build and send game state changed messages
       * @param oldState the old state
       * @param newState the new state
       */
      void SendGameStateChangedMessage(GameState &oldState, GameState &newState);

      /**
       * Helper method to initialize the scene for the intro
       * @note These methods help us to ensure that initialization can happen 
       * in one set place after all messages have been processed
       */
      void OnIntro();

      /**
       * Helper method to initialize the scene for the game
       */
      void OnGame();

      /**
       * Helper method to be called when the game is over
       */
      void OnDebrief();

      /**
       * Help method called when we change state to the menu
       */
      void OnMenu();

      /**
       * Helper method to stop any sounds that are playing
       */
      void StopSounds();

      /**
       * This function determines is an actor of specified type is 
       * located in the game map and throws and exception if not.
       * @param actor A pointer to the actor to find
       * @param throwException True if you want the function to 
       * throw an exception if the actor is not found in the map
       * @throws MISSING_REQUIRED_ACTOR_EXCEPTION 
       */
      template <class T>
      void IsActorInGameMap(T *&actor, bool throwException);

      GameState *mCurrentState;
      PlayerActor *mPlayer;

#ifdef _DEBUG
      dtCore::FlyMotionModel *mMotionModel;
#else
      dtCore::CollisionMotionModel *mMotionModel;
#endif
      
      dtAudio::Sound *mBellSound, *mDebriefSound, *mWalkSound, *mRunSound, *mCrouchSound;
      GameItemActor *mCurrentIntersectedItem;
};

template<class T>
void InputComponent::IsActorInGameMap(T *&actor, bool throwException = true) 
{
   std::vector<dtCore::RefPtr<dtGame::GameActorProxy> > proxies;
   GetGameManager()->GetAllGameActors(proxies);

   for(unsigned int i = 0; i < proxies.size(); i++)
   {
      actor = dynamic_cast<T*>(proxies[i]->GetActor());
      if(actor != NULL)
         break;
   }
   if(actor == NULL)
   {
      if(throwException)
         throw dtUtil::Exception(ExceptionEnum::MISSING_REQUIRED_ACTOR_EXCEPTION, 
            "Failed to find the actor: " + actor->GetName(), __FILE__, __LINE__);
   }
}

#endif
