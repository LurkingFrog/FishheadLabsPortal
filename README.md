# FishheadLabsPortal

The full web portal, both client and server.

## Components

### Cache

A cacheable data structure implementing GraphQL querying as a feature

### Postgres

The database backend to store the normalized data.

### Sheets

The Google Sheets backend to retrieve data.

### Client

The wasm based wrapper used to interface with a local instance of the cache and syncing it to the server.

### Portal

ReasonML web GUI, using the Client for a data backend

### Server

An interface to manage data persistence via API. It will expose Data via GraphQL queries, as well as have
some RPC function calls.

## TODO

### Milestone 1

- Setup Cargo.toml for each project and workspace
- Create simple HashMap with random values in Cache
- Derive GraphQL values as feature in Cache
- Create Server with Warp to serve Cache
- Make "get_orgs" in Client
