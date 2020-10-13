type fieldState =
  | /** Nothing has been entered into the given field */
    Empty
  | /** The value is unchanged from the default value */
    Default
  | /** The user has altered the value. */
    Changed
  | /** The current value in the field has been validated */
    Validated
  | /** The field can no longer be changed */
    Locked
  | /** This contains a validation error */
    Error(Errors.t);

type t = {
  name: string,
  fieldState,
};
