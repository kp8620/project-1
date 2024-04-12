Steps:

<!-- 1) Figure out the Command Line Arguements and Functions  -->
<!-- 2) Figure out the executable -->
3) For now read up on Forks and what Ill need to know abotu them
4) Check How to add processes into a Queue
    - If a process is not on the top, SIGSTOP
    - If it finishes, remove it
    - Else
        Put it on the back of the queue
        Put the process on top to run depending onthe quantum