#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	//sumar cost a la diagonal, si et mous en diagonal, el +1 es +arrel de 2
	g = parent->g + 1;
	h = pos.DistanceManhattan(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = 0;
	/*
	p2DynArray<iPoint> last_path_normal;
	int distance_without_portals = 0;
	int distance_from_origin_to_nearest_portal = 0;
	int distance_from_portal_to_destination = 0;
	int distance_with_portals = 0;

	int distance_to_portal_A = 0;
	int distance_to_portal_B = 0;

	distance_without_portals = origin.DistanceManhattan(destination);

	distance_to_portal_A = origin.DistanceManhattan(App->scene->potal_map_tile_1);
	distance_to_portal_B = origin.DistanceManhattan(App->scene->potal_map_tile_2);

	if (distance_to_portal_A < distance_to_portal_B) {
		distance_from_origin_to_nearest_portal = origin.DistanceManhattan(App->scene->potal_map_tile_1);
		distance_from_portal_to_destination = App->scene->potal_map_tile_2.DistanceManhattan(destination);
		nearest_portal = App->scene->potal_map_tile_1;
		furthest_portal = App->scene->potal_map_tile_2;
	}
	else {
		distance_from_origin_to_nearest_portal = origin.DistanceManhattan(App->scene->potal_map_tile_2);
		distance_from_portal_to_destination = App->scene->potal_map_tile_1.DistanceManhattan(destination);
		nearest_portal = App->scene->potal_map_tile_2;
		furthest_portal = App->scene->potal_map_tile_1;
	}
	distance_with_portals = distance_from_origin_to_nearest_portal + distance_from_portal_to_destination;

	iPoint p;
	p.SetToZero();
	last_path.Clear();
	int j = 0;

	if (distance_without_portals <= distance_with_portals) {
		ret = CreateNormalPath(origin, destination, last_path_normal);

		j = last_path_normal.Count();
	}
	else {
		int ret1 = CreateNormalPath(origin, nearest_portal, last_path_normal);

		j = last_path_normal.Count();
		for (int i = 0; i < j; i++) {
			last_path_normal.Pop(p);
			last_path.PushBack(p);
		}

		int ret2 = CreateNormalPath(furthest_portal, destination, last_path_normal);

		j = last_path_normal.Count();

		if ((ret1 == 1) && (ret2 == 1)) {
			ret = 1;
		}
		else {
			ret = -1;
		}
	}

	for (int i = 0; i < j; i++) {
		last_path_normal.Pop(p);
		last_path.PushBack(p);
	}*/

	return ret;
}

int j1PathFinding::CreateNormalPath(const iPoint& origin, const iPoint& destination, p2DynArray<iPoint>& last_path_normal)
{
	// TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(origin) == false || IsWalkable(destination) == false) {
		return -1;
	}
	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list
	PathList open;
	PathNode originNode;
	originNode.pos.x = origin.x;
	originNode.pos.y = origin.y;
	open.list.add(originNode);
	PathList close;
	while (open.list.count() != 0) {
		// TODO 3: Move the lowest score cell from open list to the closed list
		p2List_item<PathNode>*activeNode;
		activeNode = close.list.add(open.GetNodeLowestScore()->data);
		open.list.del(open.GetNodeLowestScore());
		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (close.list.end->data.pos == destination) {
			last_path_normal.Clear();
			PathNode item = activeNode->data;
			while (item.parent != nullptr) {
				last_path_normal.PushBack(item.pos);
				item = *item.parent;
			}
			last_path_normal.PushBack(iPoint(origin.x, origin.y));
			last_path_normal.Flip();
			return 1;
		}
		// TODO 5: Fill a list of all adjancent nodes
		PathList pathtemp;
		activeNode->data.FindWalkableAdjacents(pathtemp);
		// TODO 6: Iterate adjancent nodes:
		// ignore nodes in the closed list
		// If it is NOT found, calculate its F and add it to the open list
		// If it is already in the open list, check if it is a better path (compare G)
		// If it is a better path, Update the parent
		for (p2List_item<PathNode> *neitemp = pathtemp.list.start; neitemp != nullptr; neitemp = neitemp->next) {
			if (close.Find(neitemp->data.pos) == NULL) {
				neitemp->data.CalculateF(destination);
				if (p2List_item<PathNode> *otherneitemp = open.Find(neitemp->data.pos)) {
					if (neitemp->data.g < otherneitemp->data.g) {
						otherneitemp->data.parent = neitemp->data.parent;
					}
				}
				else {
					open.list.add(neitemp->data);
				}

			}
		}
	}
	return -1;
}

