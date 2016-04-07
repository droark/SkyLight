#include "map.h"
#include "pixels.h"

// Recursively move a Comet forward once
void moveCometRecursive(Pixel *px, int nw_addr){	
	if(px->next){
		moveCometRecursive(px->next, px->addr);
	} else {
		// TODO: turn off current pixel, since it won't be set by the next px in the comet
	}
	px->addr = nw_addr;
	// TODO: set color of new pixel
	return;
}


void buildCometRecursive(Pixel *px, int color, int len){
	if(len){
		px->next = malloc(sizeof());
		buildCometRecursive();
	}
}

// Determine the address of the next pixel for the comet head
bool moveComet(Comet *comet){
	moveComet(comet->head, comet->next_addr);
	
	if(comet->path_len){
		comet->next_addr += comet->incr;
		comet->path_len--;
	}
	else{
		handleNode(comet);
	}
	
	return // TODO: Determine whether the comet is dead)
}


// Handler for when a comet enters a node
void handleNode(Comet *comet){	
	// Choose the next path and hold the metadata of the chosen path
	PathMeta *new_path_meta = comet->target_node->branches[chooseNewPath()];
	
	// Get the endpoint
	int endpoint = (new_path_meta->path_info & PATH_END);
	
	// Hold the new path
	Path *new_path = new_path_meta->path;
	
	// Set the comet's new target node
	comet->target_node = new_path->end_nodes[endpoint];
	
	// Set the comet's next address
	comet->next_addr = new_path->end_px_addr[endpoint];
	
	// Set the comet's new path length
	comet->path_len = new_path->len;
}