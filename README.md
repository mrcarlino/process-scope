# Build
sudo docker build -t process-scope .

# Allow container to access host screen
xhost +local:

# Run
sudo docker compose up

# Clean up
sudo docker compose down