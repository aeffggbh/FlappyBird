#include "Game.h"

#include <iostream>

#include "raylib.h"

#include "Scenes/MainMenu.h"
#include "Scenes/Gameplay.h"
#include "Scenes/Tutorial.h"
#include "Scenes/Credits.h"
#include "Scenes/Pause.h"
#include "Scenes/GameOver.h"
#include "Scenes/HowToPlay.h"

#include "Interface/Button.h"
#include "Utils/SoundManager.h"

using namespace Buttons;
using namespace SoundManager;

namespace Game
{
	static const int screenWidth = 800;
	static const int screenHeight = 600;

	enum class CurrentScene
	{
		MainMenu, Gameplay, Tutorial, Credits, Pause, GameOver, HowToPlay
	};

	static CurrentScene currentScene;

	static float gameShouldClose;
	static float gameplayOnGoing;

	static void Init();
	static void Update();
	static void Draw();
	static void	Load();
	static void Unload();

	static void ResetGame();

	void Run()
	{
		InitWindow(screenWidth, screenHeight, "Aliens");
		Load();
		Init();

		do
		{
			Update();
			Draw();

		} while (!gameShouldClose);

		Unload();
	}


	void Init()
	{

		gameShouldClose = false;
		gameplayOnGoing = false;

		currentScene = CurrentScene::MainMenu;

		SetExitKey(0);

		MainMenu::Init();
		Gameplay::Init();
		Tutorial::Init();
		Credits::Init();
		Pause::Init();
		GameOver::Init();
		HowToPlay::Init();
	}


	void Update()
	{
		gameShouldClose = WindowShouldClose();

		switch (currentScene)
		{
		case Game::CurrentScene::MainMenu:
		{
			if (SoundManager::IsPlaying(SoundManager::Song::gameplay))
				SoundManager::Stop(SoundManager::Song::gameplay);
			MainMenu::Update();

			if (IsButtonPressed(MainMenu::play))
			{
				gameplayOnGoing = true;
				currentScene = CurrentScene::Gameplay;
				Gameplay::SetMultiplayer(false);
				SoundManager::Play(GetRandomSfx());
			}
			else if (IsButtonPressed(MainMenu::play2))
			{
				currentScene = CurrentScene::HowToPlay;
				SoundManager::Play(GetRandomSfx());

			}
			else if (IsButtonPressed(MainMenu::tutorial))
			{
				currentScene = CurrentScene::Tutorial;
				SoundManager::Play(GetRandomSfx());
			}
			else if (IsButtonPressed(MainMenu::credits))
			{
				currentScene = CurrentScene::Credits;
				SoundManager::Play(GetRandomSfx());
			}
			else if (IsButtonPressed(MainMenu::exit))
			{
				gameShouldClose = true;
			}

			break;
		}


		case Game::CurrentScene::Gameplay:
		{
			if (SoundManager::IsPlaying(SoundManager::Song::menu))
				SoundManager::Stop(SoundManager::Song::menu);

			gameplayOnGoing = Gameplay::Update();

			if (IsButtonPressed(Gameplay::pause) || IsKeyReleased(KEY_ESCAPE))
			{
				currentScene = CurrentScene::Pause;
				SoundManager::Play(GetRandomSfx());
			}

			if (!gameplayOnGoing)
			{
				/*runScore = Gameplay::GetRunScore();

				if (runScore > maxScore)
				{
					maxScore = runScore;
				}

				GameOver::SetScores(maxScore, runScore);
				*/
				currentScene = CurrentScene::GameOver;
			}

			break;
		}


		case Game::CurrentScene::Tutorial:
		{
			MainMenu::KeepMusic();

			if (IsButtonPressed(Tutorial::returnToMenu) || IsKeyReleased(KEY_ESCAPE))
			{
				currentScene = CurrentScene::MainMenu;
				SoundManager::Play(GetRandomSfx());
			}

			break;
		}


		case Game::CurrentScene::Credits:
		{
			MainMenu::KeepMusic();
			Credits::Update();

			if (IsButtonPressed(Credits::returnToMenu) || IsKeyReleased(KEY_ESCAPE))
			{
				currentScene = CurrentScene::MainMenu;
				SoundManager::Play(GetRandomSfx());
			}

			break;
		}


		case Game::CurrentScene::Pause:
		{
			if (IsButtonPressed(Pause::returnToMenu))
			{
				ResetGame();
				currentScene = CurrentScene::MainMenu;
				SoundManager::Play(GetRandomSfx());
			}
			else if (IsButtonPressed(Pause::continuePlaying) || IsKeyReleased(KEY_ESCAPE))
			{
				currentScene = CurrentScene::Gameplay;
				SoundManager::Play(GetRandomSfx());
			}
			break;
		}


		case Game::CurrentScene::GameOver:
		{
			if (IsButtonPressed(GameOver::returnToMenu))
			{
				ResetGame();
				currentScene = CurrentScene::MainMenu;
				SoundManager::Play(GetRandomSfx());
			}
			else if (IsButtonPressed(GameOver::playAgain))
			{
				ResetGame();
				currentScene = CurrentScene::Gameplay;
				SoundManager::Play(GetRandomSfx());
			}
			break;
		}
		case Game::CurrentScene::HowToPlay:
		{
			MainMenu::KeepMusic();
			if (IsButtonPressed(HowToPlay::continuePlaying))
			{
				gameplayOnGoing = true;
				currentScene = CurrentScene::Gameplay;
				Gameplay::SetMultiplayer(true);
				SoundManager::Play(GetRandomSfx());
			}
			else if (IsButtonPressed(HowToPlay::returnToMenu))
			{
				currentScene = CurrentScene::MainMenu;
				SoundManager::Play(GetRandomSfx());
			}
			break;
		}

		default:
		{
			break;
		}
		}
	}


	void Draw()
	{
		BeginDrawing();

		switch (currentScene)
		{
		case Game::CurrentScene::MainMenu:
		{
			MainMenu::Draw();
			break;
		}


		case Game::CurrentScene::Gameplay:
		{
			Gameplay::Draw();
			break;
		}


		case Game::CurrentScene::Tutorial:
		{
			Tutorial::Draw();
			break;
		}


		case Game::CurrentScene::Credits:
		{
			Credits::Draw();
			break;
		}

		case Game::CurrentScene::Pause:
		{
			Pause::Draw();
			break;
		}


		case Game::CurrentScene::GameOver:
		{
			GameOver::Draw();
			break;
		}
		case Game::CurrentScene::HowToPlay:
		{
			HowToPlay::Draw();
			break;
		}

		default:
		{
			std::cout << "Error, entro al default" << std::endl;
			break;
		}
		}

		EndDrawing();
	}

	void Load()
	{
		SoundManager::Load();
		Gameplay::Load();
	}

	void Unload()
	{
		Gameplay::Unload();
		Tutorial::Unload();
		Credits::Unload();
		GameOver::Unload();

		SoundManager::Unload();

		CloseWindow();
	}

	void ResetGame()
	{

		Gameplay::Reset();
	}
}