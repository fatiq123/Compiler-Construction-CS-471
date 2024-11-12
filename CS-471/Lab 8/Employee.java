import java.util.Objects;

public record Employee(int id, String firstName, String lastName) {
    // public Record {
    //     Objects.requireNonNull(firstName);
    //     Objects.requireNonNull(lastName);   
    // }

    public Employee(int id, String firstName, String lastName) {
        this.id = id;
        this.firstName = firstName;
        this.lastName = lastName;
    }
}
