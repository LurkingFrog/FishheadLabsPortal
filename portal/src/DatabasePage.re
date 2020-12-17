open Helpers;

module Widget = {
  [@react.component]
  let make = () => {
    <div className="Widget"> "Placeholder for Database Widget"->ReasonReact.string </div>;
  };
};

module Status = {
  [@react.component]
  let make = () => {
    <div className="Status"> "Placeholder for Status"->ReasonReact.string </div>;
  };
};

module Configuration = {
  [@react.component]
  let make = () => {
    <div className="Configuration"> "Placeholder for Configuration"->ReasonReact.string </div>;
  };
};

module IoTemplates = {
  [@react.component]
  let make = () => {
    let msg = "IoTemplates will be how we format data for export. This is going to be closely linked to the template
     engine. The only difference is this is going to be a list of mini-templates/per row as opposed to
     an entire document.";
    <NotImplemented msg />;
  };
};

module Import = {
  /** The Import Request Object */
  // [@@deriving enform_form ]
  module Request = {
    module GoogleSheet = {
      type t = {sheetId: string};
    };

    // [@@deriving enform_component]
    type source =
      | GoogleSheets(GoogleSheet.t)
      | Excel
      | CSV;

    type target =
      | Postgres;

    // [@request_root]
    type t = {
      importSource: source, // Attribute? [@default GoogleSheets]
      importTarget: target,
    };
  };

  /** Derived from importData
 *
 * This is what the derived code should looklike step one of the PPX is making the interface work
 */
  // --->  END GENERATED CODE <---
  // Name is taken from the request Type
  module ImportDataForm = {
    open Enform.Lib;

    // All derived code should only access to lib and none of the internals.
    let newImportDataForm = () => {
      newForm(~guid="ImportDataForm", ())
      |> kC((form: Enform.Form.t) => {
           // let sourceGroup = addSelectorGroup(~fields=[|sheetIdField|], sourceField.guid, "sourceGroup");
           // Create the fields
           let inputSource =
             newSelectInput(
               ~config=newConfig(~label="Source", "importSource") |> getExn,
               ~options=[|("GoogleSheets", "Google Sheets"), ("Excel", "Excel"), ("CSV", "CSV")|],
               "importSource",
             );

           // Google Sheets fields
           let gsSheetId =
             newTextInput(
               ~config=newConfig(~label="Sheet Id", "gsSheetId") |> getExn,
               ~icon="fa-pencil",
               "gsSheetId",
             );

           let gsAsUser =
             newSelectInput(
               ~config=newConfig(~label="As User", "gsAsUser") |> getExn,
               ~options=[|("sfogelson", "Dr. Susan Fogelson"), ("dfogelson", "David Fogelson")|],
               "gsAsUser",
             );

           // Excel fields
           let excelFilePath =
             newTextInput(
               ~config=newConfig(~label="File Path", "excelFilePath") |> getExn,
               "excelFilePath",
             );

           // CSV fields
           let csvFilePath =
             newTextInput(
               ~config=newConfig(~label="File Directory", "csvFilePath") |> getExn,
               "csvFilePath",
             );

           //  let clearButton = newClearButton("Clear");
           // let submitButton = newSubmitButton("Submit");

           // Group the fields and create the form
           [|inputSource, gsSheetId, gsAsUser, excelFilePath, csvFilePath|]
           |> A.flattenExn(
                ~groupErrorMsg=
                  Some(format("There were issues creating the fields for form '%s': ", form.guid)),
              )
           |> kC(addFields(form));
         })
      |> kC(
           addSelectorGroup(
             ~groupMap=[|
               ("GoogleSheets", [|"gsSheetId", "gsAsUser"|]),
               ("Excel", [|"excelFilePath"|]),
               ("CSV", [|"csvFilePath"|]),
             |],
             ~selectorId="importSource",
             ~parentId=rootGroupId,
             "importSourceGroup",
           ),
         );
    };
    // [@react.component]
    // let make = (~form ~children) => {
    //   Js.log("Running ImportDataForm.make for Database Page");
    //   let (formState, dispatch) = React.useState(() => newImportDataForm() |> Helpers.getExn);
    //   Js.log("Rendering the ImportDataForm:");
    //   Js.log(formState);
    //   let _ = dispatch;
    //   // Get the field
    //   <div className=[%tw ""]> "Placeholder for ImportDataForm"->ReasonReact.string children </div>;
    // };
    // each field gets it's own module to play in
    // module Source = {
    //   [@react.component]
    //   let make = (~formState) => {
    //     <Field form=formState fieldId="Source" />;
    //   };
    // };
    // module Target = {
    //   [@react.component]
    //   let make = (~formState) => {
    //     <Field form=formState fieldId="Target" />;
    //   };
    // };
  };
  // --->  END GENERATED CODE <---

  /** Import render function */
  [@react.component]
  let make = () => {
    let (formState, setFormState) =
      React.useState(_ => ImportDataForm.newImportDataForm() |> Helpers.getExn);

    <div className=[%tw "panel panel-default"]>
      <div className=[%tw "panel-heading"]>
        <h3 className=[%tw "panel-title"]> <strong> "Import/Export Data"->ReasonReact.string </strong> </h3>
      </div>
      <div className=[%tw "panel-body"]>
        <p>
          "We can move our data aronud to different sources. This is used for importing from Google Sheets,
           Excel, or even CSV files. Exports can be done to existing/new Google Sheets or new CSV files."
          ->ReasonReact.string
        </p>
      </div>
      <div className=[%tw "panel-body"]> <Enform.Form form=formState dispatch=setFormState /> </div>
    </div>;
  };
};

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  Js.log("Rendering Database Page because the URL is");
  Js.log(url);
  switch (url.path) {
  | [_, "configuration"] => <Configuration />
  | [_, "import"] =>
    Js.log("Path says to render import");
    <Import />;
  | [_, "io_templates"] => <IoTemplates />
  | [_, "status"]
  | [_] => <Status />
  | _ => <PageNotFound />
  };
};
