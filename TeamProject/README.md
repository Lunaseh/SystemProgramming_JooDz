# Project Overview
### ● Goal
  ○ Take items to get score.
  <br>
  ○ Disturb the opponent to prevent getting scores.
  <br>
  ○ Accumulate more scores than the opponent to win.
  <br>
  <br>
  
### ● How to take item?
  ○ Items with random scores are randomly spawned to the intersections of the map.
  <br>
  ○ You can take an item by simply reaching an intersection.
  <br>
  ○ A server lets you know where items are located.
  <br>
  <br>
  
### ● How to sabotage the opponent?
  ○ You can set a trap when you reach an intersection.
  <br>
  ○ A trap reduces the score when a player (Include you!!) reaches the intersection.
  <br>
  ○ A server lets you know which intersection has a trap.
  <br>
  ○ A server lets you know where your opponent is located.
  <br>
  <br>
  
### ● How can I control my robot?
  ○ You will set the algorithm to achieve the goals.
  <br>
  ○ Your Raspbot automatically moves along with lines based on your algorithm.
  <br>
  ○ You cannot manually manipulate your Raspbot.
  <br>
# Game Environment
### ● Play time
  ○ About 2 minutes. (Might be changed)
### ● Map
  ○ 4X4 grid with 4 rows, 4 columns and 25 intersections.
    <br>
  ○ (𝑥,𝑦) 𝑤ℎ𝑒𝑟𝑒, 0≤𝑥,𝑦≤4
    <br>
  ○ See figure 1.
### ● QR code
  ○ QRcode blocks will be placed on every single intersection.
    <br>
  ○ A QRcode gives you the location of the corresponding intersection.
    <br>
  ○ You should detect and decode QRcode to reach the intersection.
    <br>
  ○ A QRcode gives you “xy”.
    <br>
    ■ For example, if you reach the intersection (1,4), you will get 14.
      <br>
    ■ (0,0)⇒00 (3,2)⇒32
      <br>
  ○ You have to convert “xy” to two integer values.
    <br>
    ■ For example, when you get 14, convert it to int row =1, int col =4
      <br>
    ■ You’re going to use these integers to communicate with a server.
      <br>
  ○ In short, read the QR code and send the locational information to server. Then, you “reach” the intersection.
### ● Progress
  ○ An intersection can have three statuses.
    <br>
    ■ No item: Nothing will happen.
      <br>
    ■ Item: Get score
      <br>
    ■ Trap: Loss score
      <br>
  ○ When you reach an intersection, the following processes will happen
    <br>
    ■ The server might change your score depending on the status.
      <br>
    ■ The status of the intersection will be turned into “No item”.
      <br>
    ■ You can set bomb or just move on to another intersection.
### ● Initial state
  ○ Player 1 starts at (0,0) and Player 2 starts at (4,4).
    <br>
  ○ Each player has 4 traps.
    <br>
  ○ 10 items are placed on a map.
# Server
### • How to connect with a server?
  ○ You can communicate with a server by using a socket.
    <br>
  ○ A port number and address should be variable.
    <br>
  ○ You can test your own raspbot code by connecting via local host (127.0.0.1).
### • Server ⟹ You
  ○ Server gives you whole map information whenever
    <br>
    ▪ A new item is spawned.
      <br>
    ▪ The location of your opponent is changed.
      <br>
  ○ A server gives you a struct DGIST.
    <br>
    ▪ DGIST.players contain information about you and your opponent.
      <br>
    ▪ DGIST.map contains information about whole intersections.
      <br>
    ▪ You should unpack the structure to utilize the information.
### • You ⟹ Server
  ○ You must let the server know your location and your action whenever you reach an intersection.
    <br>
  ○ You give the server a ClientAction structure.
    <br>
    ▪ ClientAction.row is your x value of (x,y)
      <br>
    ▪ ClientAction.col is your y value of (x,y)
      <br>
    ▪ ClientAction.action
### • 0, if you don’t want to set a trap.
### • 1, if you want to set a trap.
### • See “server.h” in our git respiratory. It might be helpful.
