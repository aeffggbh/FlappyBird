#include "Obstacle.h"

#include <iostream>

using namespace std;

namespace Obstacle
{
	enum TreeParts
	{
		Trunk,
		TopLeaves,
		BottomLeaves
	};

	Texture2D tree;
	Sprite::Sprite trunk;
	Sprite::Sprite topLeaves;
	Sprite::Sprite bottomLeaves;

	static float screenWidth;
	static float screenHeight;

	static float obstacleWidth = 50;
	static float maxObstacleHeight;
	static float minObstacleHeight;

	static void ManageObstacle(Obstacle& obstacle);
	static void MoveObstacle(Obstacle& enemyToMove);
	static void KeepObstacleOnScreen(Obstacle& enemyToKeepOnScreen);
	static void InitTextures();

	void Load()
	{
		tree = LoadTexture("res/sprites/tree.png");
	}

	void Init(Obstacle& obstacle, float originX, float speed, Color color)
	{
		InitTextures();

		screenWidth = static_cast<float>(GetScreenWidth());
		screenHeight = static_cast<float>(GetScreenHeight());

		maxObstacleHeight = screenHeight / 2;
		minObstacleHeight = obstacleWidth;

		obstacle.color = color;

		obstacle.originX = originX;
		if (static_cast<int>(originX) >= GetScreenWidth())
			obstacle.finishX = 0;
		else
			obstacle.finishX = static_cast<float>(GetScreenWidth());

		obstacle.speed = speed;

		SetObstacle(obstacle);
	}

	void Update(Obstacle& obstacle)
	{
		ManageObstacle(obstacle);
	}

	void Draw(Obstacle obstacle)
	{
		for (int i = 0; i < obstacleParts; i++)
		{
			float trunkYStart = obstacle.parts[i].collisionShape.y;

			for (int j = 0; j < frames; j++)
			{
				if (j == Trunk)
				{
					Rectangle dest = obstacle.parts[i].spriteParts[j].destination;

					if (i == 0)
						trunkYStart = obstacle.parts[i].collisionShape.y + obstacle.parts[i].collisionShape.height - obstacle.parts[i].spriteParts[j].destination.height;
					
					for (int k = 0; k < obstacle.parts[i].trunkRepetitions; k++)
					{
						dest.y = trunkYStart;
						DrawTexturePro(obstacle.parts[i].spriteParts[j].texture, obstacle.parts[i].spriteParts[j].source, dest, { 0,0 }, 0, obstacle.color);

						if (i == 0)
							trunkYStart -= dest.height;
						else
							trunkYStart += dest.height;
					}
				}
				else
				{
					if ((i == 0 && j == TopLeaves) || i == 1 && j == BottomLeaves)
						DrawTexturePro(obstacle.parts[i].spriteParts[j].texture, obstacle.parts[i].spriteParts[j].source, obstacle.parts[i].spriteParts[j].destination, { 0,0 }, 0, obstacle.color);

				}

			}
		}
	}

	void ResetObstacle(Obstacle& obstacleToReset)
	{
		SetObstacle(obstacleToReset);
	}

	void SetObstacle(Obstacle& obstacleToSet)
	{
		float dividedObstacleHeight = static_cast<float>(GetRandomValue(static_cast<int>(minObstacleHeight), static_cast<int>(maxObstacleHeight)));

		float obstacleSpace = 200.0f;

		cout << "Set!!" << endl;
		
		for (int i = 0; i < obstacleParts; i++)
		{
			obstacleToSet.parts[i].spriteParts[Trunk] = trunk;
			obstacleToSet.parts[i].spriteParts[TopLeaves] = topLeaves;
			obstacleToSet.parts[i].spriteParts[BottomLeaves] = bottomLeaves;

			obstacleToSet.parts[i].collisionShape.width = obstacleWidth;
			if (static_cast<int>(obstacleToSet.originX) == GetScreenWidth())
				obstacleToSet.parts[i].collisionShape.x = obstacleToSet.originX + obstacleWidth;
			else
				obstacleToSet.parts[i].collisionShape.x = obstacleToSet.originX - obstacleWidth;


			for (int j = 0; j < frames; j++)
			{
				obstacleToSet.parts[i].spriteParts[j].destination.width = obstacleWidth;
				obstacleToSet.parts[i].spriteParts[j].destination.height = static_cast<float>(obstacleToSet.parts[i].spriteParts[j].texture.height);
				obstacleToSet.parts[i].spriteParts[j].destination.x = obstacleToSet.parts[i].collisionShape.x;
			}

			if (i == 0)
			{
				//top
				obstacleToSet.parts[i].collisionShape.height = dividedObstacleHeight;
				obstacleToSet.parts[i].collisionShape.y = 0;

				obstacleToSet.parts[i].spriteParts[TopLeaves].destination.y = obstacleToSet.parts[i].collisionShape.y + obstacleToSet.parts[i].collisionShape.height;
			}
			else
			{
				//bottom
				obstacleToSet.parts[i].collisionShape.y = obstacleToSet.parts[i - 1].collisionShape.y + obstacleToSet.parts[i - 1].collisionShape.height + obstacleSpace;

				obstacleToSet.parts[i].collisionShape.height = screenHeight - obstacleToSet.parts[i].collisionShape.y;

				obstacleToSet.parts[i].spriteParts[BottomLeaves].destination.y = obstacleToSet.parts[i].collisionShape.y - obstacleToSet.parts[i].spriteParts[BottomLeaves].destination.height;

			}
			obstacleToSet.parts[i].pos.x = obstacleToSet.parts[i].collisionShape.x;
			obstacleToSet.parts[i].pos.y = obstacleToSet.parts[i].collisionShape.y;

			obstacleToSet.parts[i].trunkRepetitions = static_cast<int>(obstacleToSet.parts[i].collisionShape.height) / trunk.texture.height + 1;
		}

		obstacleToSet.addedScore = false;
	}


	void ManageObstacle(Obstacle& obstacle)
	{
		MoveObstacle(obstacle);
		KeepObstacleOnScreen(obstacle);
	}

	void MoveObstacle(Obstacle& enemyToMove)
	{
		for (int i = 0; i < obstacleParts; i++)
		{
			enemyToMove.parts[i].pos.x -= enemyToMove.speed * GetFrameTime();
			enemyToMove.parts[i].collisionShape.x = enemyToMove.parts[i].pos.x;

			for (int j = 0; j < frames; j++)
				enemyToMove.parts[i].spriteParts[j].destination.x = enemyToMove.parts[i].pos.x;


		}
	}

	void KeepObstacleOnScreen(Obstacle& obstacleToKeep)
	{
		bool reset = false;

		if (static_cast<int>(obstacleToKeep.finishX) == 0)
		{
			if (obstacleToKeep.parts[0].pos.x + obstacleToKeep.parts[0].collisionShape.width < 0)
				reset = true;

		}
		else if (static_cast<int>(obstacleToKeep.parts[0].pos.x) > GetScreenWidth())
			reset = true;

		if (reset)
			ResetObstacle(obstacleToKeep);
	}

	void InitTextures()
	{
		float treeFrames = 3.0f;

		trunk.texture = tree;
		bottomLeaves.texture = tree;
		topLeaves.texture = tree;

		trunk.source.x = 0;
		trunk.source.y = 0;

		bottomLeaves.source.x = trunk.source.x + static_cast<float>(trunk.texture.width) / treeFrames;
		bottomLeaves.source.y = 0;

		topLeaves.source.x = bottomLeaves.source.x + static_cast<float>(bottomLeaves.texture.width) / treeFrames;
		topLeaves.source.y = 0;

		trunk.source.width = static_cast<float>(trunk.texture.width) / treeFrames;
		bottomLeaves.source.width = static_cast<float>(bottomLeaves.texture.width) / treeFrames;
		topLeaves.source.width = static_cast<float>(topLeaves.texture.width) / treeFrames;

		trunk.source.height = trunk.source.width;
		bottomLeaves.source.height = bottomLeaves.source.width;
		topLeaves.source.height = topLeaves.source.width;

	}

	void Unload()
	{
		UnloadTexture(tree);
	}

}
