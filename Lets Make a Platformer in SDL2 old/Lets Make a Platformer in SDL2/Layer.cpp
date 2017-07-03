﻿/*Written 2017 by Michał Przekota.
To the extent possible under law, the author(s) have dedicated all copyright to men who want to help with the problem.
*/
#include "Layer.h"

Layer::Layer()
{
}

Layer::~Layer()
{
}

std::pair<int, int> Layer::SetTiles(std::string tileString)
{
	std::pair<int, int> tiles;
	tiles.first = atoi(tileString.substr(0, tileString.find(',')).c_str());
	tiles.second = atoi(tileString.substr(tileString.find(',') + 1).c_str());

	return tiles;
}

void Layer::LoadContent(std::string layerID, std::string mapID, SDL_Renderer *renderer)
{
	file.LoadContent("Files/config/Maps/" + mapID + ".cfg", attributes, contents);
	int indexY = 0;
	for (int i = 0; i < attributes.size(); i++)
	{
		for (int j = 0; j < attributes[i].size(); j++)
		{
			std::string att = attributes[i][j];
			std::string con = contents[i][j];

			if (att == "TileSheet")
			{
				tileSheet.loadFromFile(con, renderer);
			}
			else if (att == "SolidTiles")
			{
				solidTiles.push_back(SetTiles(con));
			}
			else if (att == "StartLayer")
			{
				for (int k = 0; k < contents[i].size(); k++)
				{
					if (contents[i][k] != "---")
					{
						Tile::State tempState = Tile::State::Passive;
						std::pair<int, int> tTile = SetTiles(contents[i][k]);

						if (std::find(solidTiles.begin(), solidTiles.end(), tTile) != solidTiles.end())
						{
							tempState = Tile::State::Solid;
						}

						imageRect.x = tTile.first * 32;
						imageRect.y = tTile.second * 32;
						imageRect.w = 32;
						imageRect.h = 32;

						Tile tileInstance;
						tiles.push_back(tileInstance);

						posX = k * 32;
						posY = indexY * 32;

						tiles[tiles.size() - 1].SetContent(con, tempState, posX, posY, renderer);
					}
				}
				indexY++;
			}
		}
	}
}
void Layer::UnloadContent()
{
	file.UnloadContent();
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles[i].UnloadContent();
	}
	tileSheet.free();
}
void Layer::Update(Player &player)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles[i].Update(player);
	}
}
void Layer::Draw(SDL_Renderer *renderer)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles[i].Draw(renderer, imageRect);
	}
}