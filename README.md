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

1. - Create the cache (Orgs only)
2. - Make graphql queryable
3. - Add import_sheet function/mutation
4. Add task tracking in importer
5. Import the data from sheets
6. Start the server
7. Assert a test query of all data works without error
8. Link mutation to page on UI
9. Add tasks to cache

   - Counts/Errors
   - Status
   - Diffs (Patchwork)

10. Add polling to UI to show state of import
    - Update counter (unchanged, new, errors per sheet imported)
    - Hidden link for changes
    - Visible but collapsable link for
    - On complete, pull the updated data

**MILESTONE**

1. Add UI org viewer
