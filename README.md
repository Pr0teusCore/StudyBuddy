UDP Broadcasted Study Group System:
                Client Application
                +-----------------+
                |   Main Menu     |
                |   [Host] [Join] |<-- User chooses role
                +------+----------+
                       |
          --------------------------------
          |                              |
     [Host Mode]                   [Join Mode]
          |                              |
  Get user inputs:             Call getServers()
  - Name                       - Display list of study groups
  - Location                   - Select a group
  - Courses                    - Query options for details:
     |                       "Where?" -> Location
     |                       "What?" -> Courses
     V                       "Members?" -> Current members
Respond to incoming              |
queries as per                - Option to join → "Join=clientname"
protocol                       - Option to exit & go back
          |                              |
     Send responses           Upon joining, log action &
     using strict formatting   update the server's member list
