# Movie and Actor Search Program

## Instructions for Users

This program allows users to search for movies or actors using their names or titles. It retrieves data from three TSV files:

- **movie.titles.tsv**: Contains information about movies
- **movie.names.tsv**: Contains information about actors/actresses
- **movie.principals.tsv**: Contains information about the roles played by actors in movies

## How to Use

1. **Compile and run the program** in a compatible environment.

2. Upon starting, you'll see a menu with three options:
   - **1**: Search for movies
   - **2**: Search for actors/actresses
   - **3**: Exit the program

3. **Select an option** by entering the corresponding number (1, 2, or 3).

4. If you choose to search for movies:
   - Enter a search phrase using `+` to separate words (e.g., "Inception+2010").
   - The program will display matching movie titles, years, and genres.
   - You can select a movie to see the actors/actresses in that film or type "done" to return to the menu.

5. If you choose to search for actors:
   - Enter a search phrase (e.g., "Leonardo+DiCaprio").
   - The program will display matching actor names, professions, and birth/death years.
   - You can select an actor to see the movies they appeared in or type "done" to return to the menu.

6. To exit the program, select option 3 from the menu.

**Note**: Ensure that the TSV files are correctly formatted and located in the same directory as the executable for successful data retrieval.

Happy Searching!
