DELETE FROM titles_test
WHERE id NOT IN (
    create view tableview
    as 
    SELECT MIN(id)
    FROM titles_test
    GROUP BY emp_no
);